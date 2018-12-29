(*
 * SNU 4190.310 Programming Languages 
 * Homework "Rozetta" Skeleton
 *)

 
let trans_v : Sm5.value -> Sonata.value = function
  | Sm5.Z z  -> Sonata.Z z
  | Sm5.B b  -> Sonata.B b
  | Sm5.L _ -> raise (Sonata.Error "Invalid input program : pushing location")
  | Sm5.Unit -> Sonata.Unit
  | Sm5.R _ -> raise (Sonata.Error "Invalid input program : pushing record")

(* TODO : complete this function *)
let rec trans_obj : Sm5.obj -> Sonata.obj = function
  | Sm5.Val v -> Sonata.Val (trans_v v)
  | Sm5.Id id -> Sonata.Id id
  | Sm5.Fn (arg, command) -> 
	let readyretcmd = [Sonata.BIND "#ret"] in
	Sonata.Fn (arg,(readyretcmd @ (trans' command)))
	(* bind때문에 env에 무언가가 하나 남는게 문제. 이걸 어떻게 고치지?? env를 다 뺐다가 다시 추가할수도 없고 말이야.. *)

(* TODO : complete this function *)
and trans' : Sm5.command -> Sonata.command = function
  | Sm5.PUSH obj :: cmds -> Sonata.PUSH (trans_obj obj) :: (trans' cmds)
  | Sm5.POP :: cmds -> Sonata.POP :: (trans' cmds)
  | Sm5.STORE :: cmds -> Sonata.STORE :: (trans' cmds)
  | Sm5.LOAD :: cmds -> Sonata.LOAD :: (trans' cmds)
  | Sm5.JTR (c1, c2) :: cmds -> [Sonata.JTR (trans' (c1@cmds),trans' (c2@cmds))]
  | Sm5.MALLOC :: cmds -> Sonata.MALLOC :: (trans' cmds)
  | Sm5.BOX z :: cmds -> Sonata.BOX z :: (trans' cmds)
  | Sm5.UNBOX id :: cmds -> Sonata.UNBOX id :: (trans' cmds)
  | Sm5.BIND id :: cmds -> Sonata.BIND id :: (trans' cmds)
  | Sm5.UNBIND :: cmds -> Sonata.UNBIND :: (trans' cmds)
  | Sm5.GET ::cmds -> Sonata.GET :: (trans' cmds)
  | Sm5.PUT ::cmds -> Sonata.PUT :: (trans' cmds)
  | Sm5.CALL :: cmds ->
	  let tcmds = (trans' cmds) in
	  let readyescapeenv = (Sonata.MALLOC) :: (Sonata.BIND "#floc") :: (Sonata.MALLOC) :: (Sonata.BIND "#fval") :: (Sonata.MALLOC) :: (Sonata.BIND "#fproc") :: [] in 
	  let escapeFunction = 
	  [Sonata.PUSH (Sonata.Id "#floc") ; Sonata.STORE ; Sonata.PUSH (Sonata.Id "#fval"); Sonata.STORE ; Sonata.PUSH (Sonata.Id "#fproc") ; Sonata.STORE] in 
	  let returnFunction =
	  [Sonata.PUSH (Sonata.Id "#fproc") ; Sonata.LOAD ; Sonata.PUSH (Sonata.Id "#fval"); Sonata.LOAD ; Sonata.PUSH (Sonata.Id "#floc") ; Sonata.LOAD] in 
	  let killescapeenv = (Sonata.UNBIND) :: (Sonata.POP) :: (Sonata.UNBIND) :: (Sonata.POP) :: (Sonata.UNBIND) :: (Sonata.POP) :: [] in
	  let pushCurrent = [Sonata.PUSH (Sonata.Fn ("#dummy", killescapeenv @ [Sonata.UNBIND ; Sonata.POP] @ tcmds))] in
	  (* 돌아오는 함수에서는 dummy값에 바인딩된것도 지워준다. *)
	  readyescapeenv @ escapeFunction @ pushCurrent @ returnFunction @ killescapeenv @ [Sonata.CALL]
  | Sm5.ADD :: cmds -> Sonata.ADD :: (trans' cmds)                                       
  | Sm5.SUB :: cmds -> Sonata.SUB :: (trans' cmds)
  | Sm5.MUL :: cmds -> Sonata.MUL :: (trans' cmds)
  | Sm5.DIV :: cmds -> Sonata.DIV :: (trans' cmds)
  | Sm5.EQ :: cmds -> Sonata.EQ :: (trans' cmds)
  | Sm5.LESS :: cmds -> Sonata.LESS :: (trans' cmds)
  | Sm5.NOT :: cmds -> Sonata.NOT :: (trans' cmds)
  | [] -> 
  let returncmd = [Sonata.PUSH (Sonata.Id "#ret"); Sonata.PUSH (Sonata.Val (Sonata.Unit)) ; Sonata.MALLOC ; Sonata.UNBIND; Sonata.POP; Sonata.CALL] in
   returncmd



(* TODO : complete this function *)
(* 있는 명령어들을 다 쓰면 자동으로 명령과 환경을 꺼내오도록 해야함. 즉, Continuation 역할을 stack 혹은 메모리에서 하도록 한다. 메모리보단 stack에 넣어놓고, call할때 스택 위에다 깔아두게 하면 될듯. *)
let transs : Sm5.command -> Sonata.command = fun command ->
  match command with
  | [] -> []
  | _ -> (trans' command)

let trans : Sm5.command -> Sonata.command = fun command ->
	let readyreturnenv = [Sonata.PUSH (Sonata.Fn ("#dummy", [])); Sonata.BIND "#ret"] in 
	readyreturnenv @ (transs command) @ [Sonata.UNBIND ; Sonata.POP]
