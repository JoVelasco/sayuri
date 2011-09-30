.PHONY: all
all: test misaki

# コンパイラ。
cc = g++

# CPUアーキテクチャ。
# nativeなら自分のCPU用。
# i386なら32ビット用。
# k8なら64ビット用。
arch = native

# オブジェクトファイル。
objs = misaki_debug.o chess_def.o chess_util.o chess_board.o \
       chess_board_gen_move.o move.o game_record.o transposition_table.o \
       chess_board_analyze.o chess_board_eval.o chess_board_search.o \
       chess_board_pondering.o chess_board_eval_weights.o opening_book.o \
       pgn_parser.o

# 使用するライブラリ。
libs = -lboost_thread

# コンパイルオプション。
opts = -O3

# テスト用アプリの作成。
test: test_main.o ${objs}
	${cc} ${opts} -march=${arch} -o $@ test_main.o ${objs} ${libs}
test_main.o: test_main.cpp *.h
	${cc} ${opts} -march=${arch} -c test_main.cpp

# ライブラリを作成。
direxist = ${shell find -name "misaki"}
misaki: libmisaki.a
	${if ${direxist}, , ${shell mkdir misaki}}
	cp libmisaki.a misaki/
	cp *.h misaki/
libmisaki.a: ${objs}
	ar rcs libmisaki.a ${objs}

# オブジェクトファイルの作成。
misaki_debug.o: misaki_debug.cpp *.h
	${cc} ${opts} -march=${arch} -c misaki_debug.cpp

chess_def.o: chess_def.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_def.cpp

chess_util.o: chess_util.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_util.cpp

chess_board.o: chess_board.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board.cpp

chess_board_gen_move.o: chess_board_gen_move.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_gen_move.cpp

move.o: move.cpp *.h
	${cc} ${opts} -march=${arch} -c move.cpp

game_record.o: game_record.cpp *.h
	${cc} ${opts} -march=${arch} -c game_record.cpp

transposition_table.o: transposition_table.cpp *.h
	${cc} ${opts} -march=${arch} -c transposition_table.cpp

chess_board_analyze.o: chess_board_analyze.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_analyze.cpp

chess_board_eval.o: chess_board_eval.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_eval.cpp

chess_board_search.o: chess_board_search.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_search.cpp

chess_board_pondering.o: chess_board_pondering.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_pondering.cpp

chess_board_eval_weights.o: chess_board_eval_weights.cpp *.h
	${cc} ${opts} -march=${arch} -c chess_board_eval_weights.cpp

opening_book.o: opening_book.cpp *.h
	${cc} ${opts} -march=${arch} -c opening_book.cpp

pgn_parser.o: pgn_parser.cpp *.h
	${cc} ${opts} -march=${arch} -c pgn_parser.cpp

# クリーン。
.PHONY: clean
clean:
	rm *.o test libmisaki.a
	rm -r misaki

# Gitのクリーン。
.PHONY: gitclean
gitclean:
	git rm *.o test libmisaki.a
	git rm -r misaki
