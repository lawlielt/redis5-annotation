/* zmalloc - total amount of allocated memory aware version of malloc()
 *
 * Copyright (c) 2009-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __ZMALLOC_H
#define __ZMALLOC_H

/* Double expansion needed for stringification of macro values. */
#define __xstr(s) __str(s)
#define __str(s) #s

#if defined(USE_TCMALLOC)
#define ZMALLOC_LIB ("tcmalloc-" __xstr(TC_VERSION_MAJOR) "." __xstr(TC_VERSION_MINOR))
#include <google/tcmalloc.h>
#if (TC_VERSION_MAJOR == 1 && TC_VERSION_MINOR >= 6) || (TC_VERSION_MAJOR > 1)
#define HAVE_MALLOC_SIZE 1
#define zmalloc_size(p) tc_malloc_size(p)
#else
#error "Newer version of tcmalloc required"
#endif

#elif defined(USE_JEMALLOC)
#define ZMALLOC_LIB ("jemalloc-" __xstr(JEMALLOC_VERSION_MAJOR) "." __xstr(JEMALLOC_VERSION_MINOR) "." __xstr(JEMALLOC_VERSION_BUGFIX))
#include <jemalloc/jemalloc.h>
#if (JEMALLOC_VERSION_MAJOR == 2 && JEMALLOC_VERSION_MINOR >= 1) || (JEMALLOC_VERSION_MAJOR > 2)
#define HAVE_MALLOC_SIZE 1
#define zmalloc_size(p) je_malloc_usable_size(p)
#else
#error "Newer version of jemalloc required"
#endif

#elif defined(__APPLE__)
#include <malloc/malloc.h>
#define HAVE_MALLOC_SIZE 1
#define zmalloc_size(p) malloc_size(p)
#endif

#ifndef ZMALLOC_LIB
#define ZMALLOC_LIB "libc"
#ifdef __GLIBC__
#include <malloc.h>
#define HAVE_MALLOC_SIZE 1
#define zmalloc_size(p) malloc_usable_size(p)
#endif
#endif

/* We can enable the Redis defrag capabilities only if we are using Jemalloc
 * and the version used is our special version modified for Redis having
 * the ability to return per-allocation fragmentation hints. */
#if defined(USE_JEMALLOC) && defined(JEMALLOC_FRAG_HINT)
#define HAVE_DEFRAG
#endif

void *zmalloc(size_t size); //调用malloc申请size大小空间 @lawlielt
void *zcalloc(size_t size); //调用系统函数calloc申请内存空间 @lawlielt
void *zrealloc(void *ptr, size_t size); //原内存调整为size空间大小 @lawlielt
void zfree(void *ptr); //调用zfree释放空间 @lawlielt
char *zstrdup(const char *s); //字符串复制方法 @lawlielt
size_t zmalloc_used_memory(void); //获取当前已经占用的内存大小 @lawlielt
void zmalloc_set_oom_handler(void (*oom_handler)(size_t)); //可自定义设置内存溢出的方法 @lawlielt
//In computing, resident set size (RSS) is the portion of memory occupied by a process
// that is held in main memory (RAM). The rest of the occupied memory exists in the swap space
// or file system, either because some parts of the occupied memory were paged out,
// or because some parts of the executable were never loaded. @lawlielt
size_t zmalloc_get_rss(void); //获取RSS信息(Resident Set Size) @lawlielt
int zmalloc_get_allocator_info(size_t *allocated, size_t *active, size_t *resident); //非jemalloc返回0 @lawlielt
void set_jemalloc_bg_thread(int enable);//let jemalloc do purging asynchronously @lawlielt
int jemalloc_purge(); //purge @lawlielt
size_t zmalloc_get_private_dirty(long pid); //获得实际内存大小  Get the sum of the specified field (converted form kb to bytes) in /proc/self/smaps.
// 2.6.16后的内核, 对于查看进程内存使用分布, 更方便了. 在/proc/{pid} 路径下有一个smaps文件, 记录了进程内存使用情况,
size_t zmalloc_get_smap_bytes_by_field(char *field, long pid); //获取/proc/self/smaps 字段的字节数 @lawlielt
size_t zmalloc_get_memory_size(void); //获取物理内存大小 @lawlielt
void zlibc_free(void *ptr); //原始系统free释放方法 防止被jemalloc覆盖free方法 @lawlielt

#ifdef HAVE_DEFRAG
void zfree_no_tcache(void *ptr);
void *zmalloc_no_tcache(size_t size);
#endif

#ifndef HAVE_MALLOC_SIZE
size_t zmalloc_size(void *ptr);
size_t zmalloc_usable(void *ptr);
#else
#define zmalloc_usable(p) zmalloc_size(p)
#endif

#ifdef REDIS_TEST
int zmalloc_test(int argc, char **argv);
#endif

#endif /* __ZMALLOC_H */
