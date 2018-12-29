(*
 * SNU 4190.310 Programming Languages 
 * K-- to SM5 translator skeleton code
 *)

open K
open Sm5
module Translator = struct

  (* TODO : complete this function  *)
  (* 공략 : K- 프로그램을 실행한 결과로 나오는게 스택에 푸시되어있는 상태면 된다. *)
  let rec trans : K.program -> Sm5.command = function
    | K.NUM i -> [Sm5.PUSH (Sm5.Val (Sm5.Z i))]
	| K.TRUE -> [Sm5.PUSH (Sm5.Val (Sm5.B true))]
	| K.FALSE -> [Sm5.PUSH (Sm5.Val (Sm5.B false))]
	| K.UNIT -> [Sm5.PUSH (Sm5.Val (Sm5.Unit))]
	| K.VAR id -> [Sm5.PUSH (Sm5.Id (id))] @ [Sm5.LOAD]
    | K.ADD (e1, e2) -> trans e1 @ trans e2 @ [Sm5.ADD]
	| K.SUB (e1, e2) -> trans e1 @ trans e2 @ [Sm5.SUB]
	| K.MUL (e1, e2) -> trans e1 @ trans e2 @ [Sm5.MUL]
	| K.DIV (e1, e2) -> trans e1 @ trans e2 @ [Sm5.DIV]
	| K.EQUAL (e1, e2) -> trans e1 @ trans e2 @ [Sm5.EQ]
	| K.LESS (e1, e2) -> trans e1 @ trans e2 @ [Sm5.LESS]
	| K.NOT (exp) -> trans exp @ [Sm5.NOT]
	| K.ASSIGN (id, exp) -> trans exp @ [Sm5.PUSH (Sm5.Id (id))] @ [Sm5.STORE] @ [Sm5.PUSH (Sm5.Id (id))] @ [Sm5.LOAD]
	| K.SEQ (e1, e2) -> (trans e1) @ [Sm5.POP] @ (trans e2)
	| K.IF (condexp, texp, fexp) -> (trans condexp) @ [Sm5.JTR ((trans texp),(trans fexp))]
	| K.WHILE (condexp, bodyexp) ->
		let pushWhile = [Sm5.PUSH (Sm5.Fn ("#whiledummy",[Sm5.BIND "#whileloop"] @ (trans condexp) @ [Sm5.JTR ((trans bodyexp) @ [Sm5.POP] @ (trans (K.CALLV ("#whileloop",K.UNIT))), [(Sm5.PUSH (Sm5.Val (Sm5.Unit)))])]))] in
		pushWhile
		@ [Sm5.BIND "#whileloop"] 
		@ (trans (K.CALLV ("#whileloop",K.UNIT)))
		@ [Sm5.UNBIND; Sm5.POP]
	| K.FOR (id,initexp,endexp,bodyexp) ->
		let makeForenv = (trans initexp) @ (trans endexp) @ [Sm5.MALLOC; Sm5.BIND "#endval"; Sm5.PUSH (Sm5.Id "#endval"); Sm5.STORE;Sm5.MALLOC; Sm5.BIND "#initval"; Sm5.PUSH (Sm5.Id "#initval"); Sm5.STORE] in
		let forTrueCommand = [Sm5.PUSH (Sm5.Id ("#initval")); Sm5.LOAD; Sm5.PUSH (Sm5.Id id); Sm5.STORE] @ (trans bodyexp) @ [Sm5.POP; (Sm5.PUSH (Sm5.Id "#initval")); Sm5.LOAD; Sm5.PUSH (Sm5.Val (Sm5.Z 1)); Sm5.ADD; (Sm5.PUSH (Sm5.Id "#initval")); Sm5.STORE] @ (trans (K.CALLV ("#forloop",K.UNIT))) in
		let forFalseCommand = [Sm5.PUSH (Sm5.Val (Sm5.Unit))] in 
		let forCommand = [Sm5.PUSH (Sm5.Id "#endval") ; Sm5.LOAD ; Sm5.PUSH (Sm5.Id "#initval") ; Sm5.LOAD ; Sm5.LESS ; Sm5.NOT ; Sm5.JTR (forTrueCommand,forFalseCommand)] in 
		let pushFor = [Sm5.PUSH (Sm5.Fn ("#fordummy",[Sm5.BIND "#forloop"] @ forCommand))] in 
		makeForenv @ pushFor @ [Sm5.BIND "#forloop"]
		@ (trans (K.CALLV ("#forloop", K.UNIT)))
		@ [Sm5.UNBIND; Sm5.POP; Sm5.UNBIND; Sm5.POP; Sm5.UNBIND; Sm5.POP]
	 (* call 할때 recursive 함수에 대한 정보를 추가로 stack에다 넣어주고, LETF에서 call 한다음 추가하도록 함수를 짜기 *)
    | K.LETV (x, e1, e2) ->
      trans e1 @ [Sm5.MALLOC; Sm5.BIND x; Sm5.PUSH (Sm5.Id x); Sm5.STORE] @
      trans e2 @ [Sm5.UNBIND; Sm5.POP]
	| K.LETF (fid,aid,funexp,executeexp) -> 
	   [(Sm5.PUSH (Sm5.Fn (aid,[Sm5.BIND fid] @ (trans funexp))))]
	   @ [Sm5.BIND fid] 
	   @ trans executeexp @ [Sm5.UNBIND; Sm5.POP]
	
	| K.CALLV (fid, argexp) -> 
	  [Sm5.PUSH (Sm5.Id fid)] @ (* save function *)
	  [Sm5.PUSH (Sm5.Id fid)] @ (trans argexp) @ [Sm5.MALLOC;Sm5.CALL]
	  
	| K.CALLR (fid, aid) -> 
	  [Sm5.PUSH (Sm5.Id fid)] @ (* save function *)
	  [Sm5.PUSH (Sm5.Id fid)] @ [Sm5.PUSH (Sm5.Id aid);Sm5.LOAD;Sm5.PUSH (Sm5.Id aid);Sm5.CALL] 
	  
    | K.READ x -> [Sm5.GET; Sm5.PUSH (Sm5.Id x); Sm5.STORE; Sm5.PUSH (Sm5.Id x); Sm5.LOAD]
    | K.WRITE x -> (trans x) @ [Sm5.MALLOC; Sm5.BIND "#temp"; Sm5.PUSH (Sm5.Id "#temp");Sm5.STORE] @
					[Sm5.PUSH (Sm5.Id "#temp");Sm5.LOAD;Sm5.PUT;Sm5.PUSH (Sm5.Id "#temp");Sm5.LOAD;Sm5.UNBIND;Sm5.POP] 

end
