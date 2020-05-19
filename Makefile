TEST_SQL=test/cli.txt

.PHONY: func


build: 
	@./build.sh
	@/usr/bin/gcc -Wno-unused-variable -Wall -g  builds/ddb.c -o builds/ddbc

build-profile: 
	@/usr/bin/gcc -Wno-unused-variable -Wall -pg  builds/ddb.c  -o builds/ddbc

run:
	@echo builds/ddbc $(TEST_SQL)
	@builds/ddbc $(TEST_SQL)

go: build run


profile: build-profile run
	@gprof builds/ddbc profiles/gmon.out > profiles/profile.txt

watch-time:
	@watch -n .3 'time make run  >/dev/null'

pipe:
	@cat test/sql/cli.txt | builds/ddbc 

cli:
	@builds/ddbc $(TEST_SQL)
