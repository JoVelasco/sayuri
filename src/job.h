/*
   job.h: マルチスレッド探索用仕事クラス。

   The MIT License (MIT)

   Copyright (c) 2013 Ishibashi Hironori

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <mutex>
#include <condition_variable>
#include "chess_def.h"
#include "chess_engine.h"
#include "transposition_table.h"
#include "pv_line.h"
#include "move_maker.h"

namespace Sayuri {
  class ChessEngine;
  class MoveMaker;

  // マルチスレッド探索用の仕事クラス。
  class Job {
    public:
      /**************************/
      /* コンストラクタと代入。 */
      /**************************/
      // [引数]
      // maker: 仕事用の手が入っているムーブメーカー。
      // client: 親チェスエンジン。
      // pos_hash: 現在の局面のハッシュ。
      // depth: 現在の深さ。
      // level: 現在のレベル。
      // alpha: 現在のアルファ値の変数。(更新される。)
      // beta: 現在のベータ値の変数。(更新される。)
      // delta: ルート探索時、ベータ値の増分の変数。(更新される。)
      // table: トランスポジションテーブル。
      // pv_lien: 現在のノードのPVライン。
      // is_null_searching: ヌルムーブの探索中かどうか。
      // is_reduced_by_null: Null Move Reductionでリダクションされたかどうか。
      // num_serached_moves: いくつ手を探索したかの変数。(更新される。)
      // material: 現在のマテリアル。
      // is_checked: 現在チェックされているかどうか。
      // moves_to_search_ptr: ルートで探索すべき手のベクトル。ないならnullptr。
      // root_move_vec_ptr: ルートで作成した手のベクトル。ないならnullptr。
      Job(MoveMaker& maker, ChessEngine& client, Hash pos_hash,
      int depth, int level, int& alpha, int& beta, int& delta,
      TranspositionTable& table, PVLine& pv_line, bool is_null_searching,
      bool is_reduced_by_null, int& num_searched_moves, int material,
      bool is_checked, std::vector<Move>* moves_to_search_ptr,
      std::vector<Move>* root_move_vec_ptr);

      Job(const Job& queue);
      Job(Job&& queue);
      Job& operator=(const Job& queue);
      Job& operator=(Job&& queue);
      virtual ~Job() {}
      Job() = delete;

      /********************/
      /* パブリック関数。 */
      /********************/
      // 手を得る。
      // [戻り値]
      // 手。
      Move PickMove();
      // ヘルパーの数を一つ増やす。
      void CountHelper();
      // 仕事終了の合図を出す。
      void FinishMyJob();
      // ヘルパーが全員仕事を終えるまで待機する。
      void WaitForHelpers();

      /**************/
      /* アクセサ。 */
      /**************/
      ChessEngine& client() {return *client_ptr_;}
      Hash pos_hash() const {return pos_hash_;}
      int depth() const {return depth_;}
      int level() const {return level_;}
      int& alpha() {return *alpha_ptr_;}
      int& beta() {return *beta_ptr_;}
      int& delta() {return *delta_ptr_;}
      TranspositionTable& table() {return *table_ptr_;}
      PVLine& pv_line() {return *pv_line_ptr_;}
      bool is_null_searching() const {return is_null_searching_;}
      bool is_reduced_by_null() const {return is_reduced_by_null_;}
      int& searched_moves() {return *num_searched_moves_ptr_;}
      int material() const {return material_;}
      bool is_checked() const {return is_checked_;}
      std::vector<Move>* moves_to_search_ptr() {return moves_to_search_ptr_;}
      std::vector<Move>* root_move_vec_ptr() {return root_move_vec_ptr_;}

    private:
      /****************/
      /* メンバ変数。 */
      /****************/
      // 親スレッドより情報。
      ChessEngine* client_ptr_;
      Hash pos_hash_;
      int depth_;
      int level_;
      int* alpha_ptr_;
      int* beta_ptr_;
      int* delta_ptr_;
      TranspositionTable* table_ptr_;
      PVLine* pv_line_ptr_;
      bool is_null_searching_;
      bool is_reduced_by_null_;
      int* num_searched_moves_ptr_;
      int material_;
      bool is_checked_;
      std::vector<Move>* moves_to_search_ptr_;
      std::vector<Move>* root_move_vec_ptr_;

      // 仕事用ムーブメーカー。
      MoveMaker* maker_ptr_;
      // ヘルパーカウンター。
      volatile int helper_counter_;
      // ミューテックス。
      std::mutex mutex_;
      // コンディション変数。
      std::condition_variable cond_;
  };
}  // namespace Sayuri

#endif
