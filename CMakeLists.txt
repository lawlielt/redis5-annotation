cmake_minimum_required(VERSION 3.14)
project(redis)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

get_filename_component(REDIS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}" ABSOLUTE)

add_subdirectory(deps)
add_subdirectory(src/modules)

set(SRC_SERVER_TMP
        src/acl.c
        src/tls.c
        src/listpack.c
        src/localtime.c
        src/gopher.c
        src/sha256.c
        src/lolwut.c
        src/lolwut5.c
        src/lolwut6.c
        src/connection.c
        src/tracking.c
        src/t_stream.c
        src/adlist.c
        src/ae.c
        src/anet.c
        src/dict.c
        src/sds.c
        src/zmalloc.c
        src/lzf_c.c
        src/lzf_d.c
        src/pqsort.c
        src/zipmap.c
        src/sha1.c
        src/ziplist.c
        src/release.c
        src/networking.c
        src/util.c
        src/object.c
        src/db.c
        src/replication.c
        src/rdb.c
        src/t_string.c
        src/t_list.c
        src/t_set.c
        src/t_zset.c
        src/evict.c
        src/defrag.c
        src/module.c
        src/quicklist.c
        src/expire.c
        src/childinfo.c
        src/redis-check-aof.c
        src/redis-check-rdb.c
        src/lazyfree.c
        src/geohash.c
        src/rax.c
        src/geohash_helper.c
        src/siphash.c
        src/geo.c
        src/t_hash.c
        src/config.c
        src/aof.c
        src/pubsub.c
        src/multi.c
        src/debug.c
        src/sort.c
        src/intset.c
        src/syncio.c
        src/cluster.c
        src/crc16.c
        src/endianconv.c
        src/slowlog.c
        src/scripting.c
        src/bio.c
        src/rio.c
        src/rand.c
        src/memtest.c
        src/crc64.c
        src/bitops.c
        src/sentinel.c
        src/notify.c
        src/setproctitle.c
        src/blocked.c
        src/hyperloglog.c
        src/latency.c
        src/sparkline.c
        )
set(SRC_SERVER src/server.c ${SRC_SERVER_TMP})

set(SRC_CLI
        src/anet.c
        src/sds.c
        src/adlist.c
        src/redis-cli.c
        src/zmalloc.c
        src/release.c
        src/anet.c
        src/ae.c
        src/crc64.c
        src/crc16.c
        src/dict.c
        src/siphash.c
        )

set(SRC_LAWLIELT_TEST
        src/dict.c
        src/siphash.c
        src/sds.c
        src/zmalloc.c
        )


set(EXECUTABLE_OUTPUT_PATH src)
link_directories(deps/linenoise/ deps/lua/src deps/hiredis)

add_executable(redis-server ${SRC_SERVER})
target_include_directories(redis-server
        PRIVATE ${REDIS_ROOT}/deps/linenoise
        PRIVATE ${REDIS_ROOT}/deps/hiredis
        PRIVATE ${REDIS_ROOT}/deps/lua/src)
target_link_libraries(redis-server
        PRIVATE pthread
        PRIVATE m
        PRIVATE lua
        PRIVATE linenoise
        PRIVATE hiredis)


add_executable(redis-cli ${SRC_CLI})
target_include_directories(redis-cli
        PRIVATE ${REDIS_ROOT}/deps/linenoise
        PRIVATE ${REDIS_ROOT}/deps/hiredis
        PRIVATE ${REDIS_ROOT}/deps/lua/src)

target_link_libraries(redis-cli
        PRIVATE pthread
        PRIVATE m
        PRIVATE linenoise
        PRIVATE hiredis)

add_executable(lawlielt_test ${SRC_LAWLIELT_TEST})
target_include_directories(lawlielt_test
        PRIVATE ${REDIS_ROOT}/deps/linenoise
        PRIVATE ${REDIS_ROOT}/deps/hiredis
        PRIVATE ${REDIS_ROOT}/deps/lua/src)

target_link_libraries(lawlielt_test
        PRIVATE pthread
        PRIVATE m
        PRIVATE linenoise
        PRIVATE hiredis)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D DICT_BENCHMARK_MAIN")