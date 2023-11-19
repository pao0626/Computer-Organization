// See LICENSE for license details.

#include "cachesim.h"
#include "common.h"
#include <cstdlib>
#include <iostream>
#include <iomanip>

// ---------------- 以下都在定義 class cache_sim_t 的 functions  ---------------//

//構造函數，初始化
cache_sim_t::cache_sim_t(size_t _sets, size_t _ways, size_t _linesz, const char* _name)
: sets(_sets), ways(_ways), linesz(_linesz), name(_name), log(false)
{
  init();
}

//error messages
static void help()
{
  std::cerr << "Cache configurations must be of the form" << std::endl;
  std::cerr << "  sets:ways:blocksize" << std::endl;
  std::cerr << "where sets, ways, and blocksize are positive integers, with" << std::endl;
  std::cerr << "sets and blocksize both powers of two and blocksize at least 8." << std::endl;
  exit(1);
}

//接受config參數，判斷條件後，傳遞參數建立cache
cache_sim_t* cache_sim_t::construct(const char* config, const char* name)
{
  const char* wp = strchr(config, ':');
  if (!wp++) help();
  const char* bp = strchr(wp, ':');
  if (!bp++) help();

  size_t sets = atoi(std::string(config, wp).c_str());
  size_t ways = atoi(std::string(wp, bp).c_str());
  size_t linesz = atoi(bp);

  //判斷是否為fully-associative cache
  if (ways > 4 /* empirical */ && sets == 1)
    return new fa_cache_sim_t(ways, linesz, name);
  return new cache_sim_t(sets, ways, linesz, name);
}

//初始化函示，判斷是否為2的冪次且不為0
void cache_sim_t::init()
{
  if (sets == 0 || (sets & (sets-1)))
    help();
  if (linesz < 8 || (linesz & (linesz-1)))
    help();

  idx_shift = 0;
  for (size_t x = linesz; x>1; x >>= 1)
    idx_shift++;

  tags = new uint64_t[sets*ways](); //cache的大小為sets*ways
  read_accesses = 0;
  read_misses = 0;
  bytes_read = 0;
  write_accesses = 0;
  write_misses = 0;
  bytes_written = 0;
  writebacks = 0;

  miss_handler = NULL;
}

//複製用
cache_sim_t::cache_sim_t(const cache_sim_t& rhs)
 : sets(rhs.sets), ways(rhs.ways), linesz(rhs.linesz),
   idx_shift(rhs.idx_shift), name(rhs.name), log(false)
{
  tags = new uint64_t[sets*ways];
  memcpy(tags, rhs.tags, sets*ways*sizeof(uint64_t));
}

//釋放記憶體
cache_sim_t::~cache_sim_t()
{
  print_stats();
  delete [] tags;
}

//印出結果
void cache_sim_t::print_stats()
{
  if (read_accesses + write_accesses == 0)
    return;

  float mr = 100.0f*(read_misses+write_misses)/(read_accesses+write_accesses);

  std::cout << std::setprecision(3) << std::fixed;
  std::cout << name << " ";
  std::cout << "Bytes Read:            " << bytes_read << std::endl;
  std::cout << name << " ";
  std::cout << "Bytes Written:         " << bytes_written << std::endl;
  std::cout << name << " ";
  std::cout << "Read Accesses:         " << read_accesses << std::endl;
  std::cout << name << " ";
  std::cout << "Write Accesses:        " << write_accesses << std::endl;
  std::cout << name << " ";
  std::cout << "Read Misses:           " << read_misses << std::endl;
  std::cout << name << " ";
  std::cout << "Write Misses:          " << write_misses << std::endl;
  std::cout << name << " ";
  std::cout << "Writebacks:            " << writebacks << std::endl;
  std::cout << name << " ";
  std::cout << "Miss Rate:             " << mr << '%' << std::endl;
}

//檢查tags
//Address 由 | tag | index | offset | 組成
uint64_t* cache_sim_t::check_tag(uint64_t addr)
{
  size_t idx = (addr >> idx_shift) & (sets-1);  //右移將 offset 去除，AND (sets-1) 留下 index 的 bits
  size_t tag = (addr >> idx_shift) | VALID; //把Valid bit加入

  for (size_t i = 0; i < ways; i++) //檢查某index中每個block
    if (tag == (tags[idx*ways + i] & ~DIRTY)) //hit
      return &tags[idx*ways + i];

  return NULL;
}

//找替換block
uint64_t cache_sim_t::victimize(uint64_t addr)
{
  size_t idx = (addr >> idx_shift) & (sets-1);
  size_t way = lfsr.next() % ways;  //LFSR隨機存取
  uint64_t victim = tags[idx*ways + way]; //取出替換
  tags[idx*ways + way] = (addr >> idx_shift) | VALID; //替換block存入新值
  return victim;
}

void cache_sim_t::access(uint64_t addr, size_t bytes, bool store)
{
  store ? write_accesses++ : read_accesses++; //cache hit，則根據訪問類型（讀取或寫入），增加相應的命中計數
  (store ? bytes_written : bytes_read) += bytes;

  uint64_t* hit_way = check_tag(addr);  // 檢查該地址是否在 cache 中。
  if (likely(hit_way != NULL))  // 如果 cache hit，且是寫入操作，則設置 Dirty bit
  {
    if (store)
      *hit_way |= DIRTY;
    return;
  }

  store ? write_misses++ : read_misses++; //cache miss，則根據訪問類型（讀取或寫入），增加相應的未命中計數
  if (log)
  {
    std::cerr << name << " "
              << (store ? "write" : "read") << " miss 0x"
              << std::hex << addr << std::endl;
  }

  uint64_t victim = victimize(addr); // 如果 cache miss，則選擇一個受害者來替換

  // 如果受害者是有效的並且是 dirty 的，則將其寫回到下一級 cache 或主記憶體，並增加寫回計數
  if ((victim & (VALID | DIRTY)) == (VALID | DIRTY))
  {
    uint64_t dirty_addr = (victim & ~(VALID | DIRTY)) << idx_shift;
    if (miss_handler)
      miss_handler->access(dirty_addr, linesz, true);
    writebacks++;
  }

  // 從下一級 cache 或主記憶體讀取新的資料。
  if (miss_handler)
    miss_handler->access(addr & ~(linesz-1), linesz, false);
  
  // 如果是寫入操作，則設置新資料的 dirty 位。
  if (store)
    *check_tag(addr) |= DIRTY;
}

void cache_sim_t::clean_invalidate(uint64_t addr, size_t bytes, bool clean, bool inval)
{
  uint64_t start_addr = addr & ~(linesz-1);
  uint64_t end_addr = (addr + bytes + linesz-1) & ~(linesz-1);
  uint64_t cur_addr = start_addr;
  while (cur_addr < end_addr) {
    uint64_t* hit_way = check_tag(cur_addr);
    if (likely(hit_way != NULL))
    {
      if (clean) {
        if (*hit_way & DIRTY) {
          writebacks++;
          *hit_way &= ~DIRTY;
        }
      }

      if (inval)
        *hit_way &= ~VALID;
    }
    cur_addr += linesz;
  }
  if (miss_handler)
    miss_handler->clean_invalidate(addr, bytes, clean, inval);
}

// --------------- 以下都在定義 fully-associative cache 的 functions  ---------------//

fa_cache_sim_t::fa_cache_sim_t(size_t ways, size_t linesz, const char* name)
  : cache_sim_t(1, ways, linesz, name)
{
}

uint64_t* fa_cache_sim_t::check_tag(uint64_t addr)
{
  auto it = tags.find(addr >> idx_shift);
  return it == tags.end() ? NULL : &it->second;
}

uint64_t fa_cache_sim_t::victimize(uint64_t addr)
{
  uint64_t old_tag = 0;
  if (tags.size() == ways)  // 如果快取已經滿了
  {
    auto it = tags.begin();
    std::advance(it, lfsr.next() % ways); //隨機找替換block
    old_tag = it->second; //it->first 得到 key，it->second 得到 value
    tags.erase(it);
  }
  tags[addr >> idx_shift] = (addr >> idx_shift) | VALID; //存入新值
  return old_tag;
}
