.PHONY: all blockMatch bugInsert markBBL util varFind testMarkBBL testVarFind testBlockMatch testBugInsert

all: blockMatch bugInsert markBBL util varFind

blockMatch:
	make TARGET -C blockMatch

bugInsert:
	make -C bugInsert

markBBL:
	make -C markBBL

util:
	make -C util

varFind:
	make TARGET -C varFind

testMarkBBL: script/markBBL.py lib/libMarkBBL.so
	cd test && python3 ../script/markBBL.py test.c -o insTest

testVarFind: script/varFind.sh lib/varFind.so
	cd test && bash ../script/varFind.sh ../test/insTest && python3 ../script/wrangVar.py

testBlockMatch: script/blockMatch.sh lib/blockMatch.so
	cd test && bash ../script/blockMatch.sh ../test/insTest && python3 ../script/wrangBlock.py

testBugInsert: script/bugInsert.py lib/libBugInsertion.so
	cd test && python3 ../script/bugInsert.py test.c -o resTest


clean:
	make clean -C blockMatch && \
	make clean -C bugInsert && \
	make clean -C markBBL && \
	make clean -C util && \
	make clean -C varFind && \
	rm -rf lib/*

