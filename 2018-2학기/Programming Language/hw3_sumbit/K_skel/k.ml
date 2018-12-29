(*
 * SNU 4190.310 Programming Languages 2018 Fall
 *  K- Interpreter Skeleton Code
 *)

(* Location Signature *)
module type LOC =
sig
  type t
  val base : t
  val equal : t -> t -> bool
  val diff : t -> t -> int
  val increase : t -> int -> t
end

module Loc : LOC =
struct
  type t = Location of int
  let base = Location(0)
  let equal (Location(a)) (Location(b)) = (a = b)
  let diff (Location(a)) (Location(b)) = a - b
  let increase (Location(base)) n = Location(base+n)
end

(* Memory Signature *)
module type MEM = 
sig
  type 'a t
  exception Not_allocated
  exception Not_initialized
  val empty : 'a t (* get empty memory *)
  val load : 'a t -> Loc.t  -> 'a (* load value : Mem.load mem loc => value *)
  val store : 'a t -> Loc.t -> 'a -> 'a t (* save value : Mem.store mem loc value => mem' *)
  val alloc : 'a t -> Loc.t * 'a t (* get fresh memory cell : Mem.alloc mem => (loc, mem') *)
end

(* Environment Signature *)
module type ENV =
sig
  type ('a, 'b) t
  exception Not_bound
  val empty : ('a, 'b) t (* get empty environment *)
  val lookup : ('a, 'b) t -> 'a -> 'b (* lookup environment : Env.lookup env key => content *)
  val bind : ('a, 'b) t -> 'a -> 'b -> ('a, 'b) t  (* id binding : Env.bind env key content => env'*)
end

(* Memory Implementation *)
module Mem : MEM =
struct
  exception Not_allocated
  exception Not_initialized
  type 'a content = V of 'a | U  (* content is single value or unit *)
  type 'a t = M of Loc.t * 'a content list (* Memory has location and it's content (it could be record *)
  let empty = M (Loc.base,[])  (* empty memory *)

  let rec replace_nth = fun l n c -> (* l : mem's content list, n : index , c : content to replace*)
    match l with
    | h::t -> if n = 1 then c :: t else h :: (replace_nth t (n - 1) c)
    | [] -> raise Not_allocated

  let load (M (boundary,storage)) loc =  (* get value of memory by location *)
    match (List.nth storage ((Loc.diff boundary loc) - 1)) with
    | V v -> v 
    | U -> raise Not_initialized

  let store (M (boundary,storage)) loc content = (* set value of memory by location *)
    M (boundary, replace_nth storage (Loc.diff boundary loc) (V content))

  let alloc (M (boundary,storage)) = (* add free space to memory *)
    (boundary, M (Loc.increase boundary 1, U :: storage))
end

(* Environment Implementation *)
module Env : ENV=
struct
  exception Not_bound
  type ('a, 'b) t = E of ('a -> 'b)
  let empty = E (fun x -> raise Not_bound)
  let lookup (E (env)) id = env id
  let bind (E (env)) id loc = E (fun x -> if x = id then loc else env x)
end

(*
 * K- Interpreter
 *)
module type KMINUS =
sig
  exception Error of string
  type id = string
  type exp =
    | NUM of int | TRUE | FALSE | UNIT (* constant *)
    | VAR of id  (* variable *)
    | ADD of exp * exp
    | SUB of exp * exp
    | MUL of exp * exp
    | DIV of exp * exp
    | EQUAL of exp * exp
    | LESS of exp * exp
    | NOT of exp
    | SEQ of exp * exp            (* sequence *)
    | IF of exp * exp * exp       (* if-then-else *)
    | WHILE of exp * exp          (* while loop *)
    | LETV of id * exp * exp      (* variable binding *)
    | LETF of id * id list * exp * exp (* procedure binding *)
    | CALLV of id * exp list      (* call by value *)
    | CALLR of id * id list       (* call by referenece *)
    | RECORD of (id * exp) list   (* record construction *)
    | FIELD of exp * id           (* access record field *)
    | ASSIGN of id * exp          (* assgin to variable *)
    | ASSIGNF of exp * id * exp   (* assign to record field *)
    | READ of id
    | WRITE of exp
    
  type program = exp
  type memory
  type env
  type value =
    | Num of int
    | Bool of bool
    | Unit
    | Record of (id -> Loc.t)
  val emptyMemory : memory
  val emptyEnv : env
  val run : memory * env * program -> value
end

module K : KMINUS =
struct
  exception Error of string

  type id = string
  type exp =
    | NUM of int | TRUE | FALSE | UNIT
    | VAR of id
    | ADD of exp * exp
    | SUB of exp * exp
    | MUL of exp * exp
    | DIV of exp * exp
    | EQUAL of exp * exp
    | LESS of exp * exp
    | NOT of exp
    | SEQ of exp * exp            (* sequence *)
    | IF of exp * exp * exp       (* if-then-else *)
    | WHILE of exp * exp          (* while loop *)
    | LETV of id * exp * exp      (* variable binding *)
    | LETF of id * id list * exp * exp (* procedure binding *)
    | CALLV of id * exp list      (* call by value *)
    | CALLR of id * id list       (* call by referenece *)
    | RECORD of (id * exp) list   (* record construction *)
    | FIELD of exp * id           (* access record field *)
    | ASSIGN of id * exp          (* assgin to variable *)
    | ASSIGNF of exp * id * exp   (* assign to record field *)
    | READ of id
    | WRITE of exp

  type program = exp

  type value =
    | Num of int
    | Bool of bool
    | Unit
    | Record of (id -> Loc.t)
	
    
  type memory = value Mem.t
  type env = (id, env_entry) Env.t
  and  env_entry = Addr of Loc.t | Proc of id list * exp * env

  let emptyMemory = Mem.empty
  let emptyEnv = Env.empty

  let value_int (v : value) : int =
    match v with
    | Num n -> n
    | _ -> raise (Error "TypeError : not int")

  let value_bool (v : value) : bool =
    match v with
    | Bool b -> b
    | _ -> raise (Error "TypeError : not bool")

  let value_unit (v : value) : unit =
    match v with
    | Unit -> ()
    | _ -> raise (Error "TypeError : not unit")

  let value_record (v : value) : (id -> Loc.t) =
    match v with
    | Record r -> r
    | _ -> raise (Error "TypeError : not record")

  let lookup_env_loc (e : env) (x : id) : Loc.t =
    try
      (match Env.lookup e x with
      | Addr l -> l
      | Proc _ -> raise (Error "TypeError : not addr")) 
    with Env.Not_bound -> raise (Error "Unbound")

  let lookup_env_proc (e : env) (f : id) : (id list * exp * env)=
    try
      (match Env.lookup e f with
      | Addr _ -> raise (Error "TypeError : not proc") 
      | Proc (id_list, exp, env) -> (id_list, exp, env))
    with Env.Not_bound -> raise (Error "Unbound")
	
	
	
  let emptyRecord : value= Record (fun x -> raise (Error "Unbound"))
  let recordLookup (record : value ) (rid : id) : Loc.t= 
                match record with
				| Record rcd -> rcd rid
				| _ -> raise (Error "TypeError : this is not record")
  let recordBind (record : value) (rid : id) (loc : Loc.t) : value = 
		        match record with
				| Record rcd -> Record (fun x -> if x = rid then loc else rcd x)
				| _ -> raise (Error "TypeError : this is not record")
				   
	  
  let rec eval mem env e =
    match e with
    | READ x -> 
      let v = Num (read_int()) in
      let l = lookup_env_loc env x in
      (v, Mem.store mem l v)
    | WRITE e ->
      let (v, mem') = eval mem env e in
      let n = value_int v in
      let _ = print_endline (string_of_int n) in
      (v, mem')
    | LETV (x, e1, e2) ->  (* 메모리에 공간을 만든다음,환경에 x를 저장, 환경이 그 메모리의 주소를, 메모리의 공간에 식의 값을 넣는다 *)
      let (v, mem') = eval mem env e1 in
      let (l, mem'') = Mem.alloc mem' in
      eval (Mem.store mem'' l v) (Env.bind env x (Addr l)) e2
    | ASSIGN (x, e) ->  (* x에 e를 실행한 결과를 메모리에 저장한다 *)
      let (v, mem') = eval mem env e in
      let l = lookup_env_loc env x in
      (v, Mem.store mem' l v)
	| LETF (f,arglist,e1,e2) ->  (* 펑션을 환경에 정의해줌 *)
	  eval mem (Env.bind env f (Proc (arglist,e1,env))) e2 
	| ASSIGNF (rexp,x,calexp) ->  (* 메모리에 존재하는 필드의 값을 바꾼다. *)
	  let (record,mem') = eval mem env rexp in
	  let (v,mem'') = eval mem' env calexp in
	  (v,(Mem.store mem'' ((value_record record) x) v))
	  (*record implementation must raise exception *)
	  
	| FIELD (innerexp,x) -> (* 필드의 값을 가져옴 *)
	  let (record, mem') = eval mem env innerexp in
	  ((Mem.load mem' ((value_record record) x)),mem')

	| RECORD r -> (* 레코드 생성 *)
		(match r with 
		 | [] -> (emptyRecord,mem)
		 | (x,hdexp)::tl -> 
			   let (v,mem') = eval mem env hdexp in
			   let (l,mem'') = Mem.alloc mem' in
			   let (record,mem''') = eval (Mem.store mem'' l v) env (RECORD tl) in
			   ((recordBind record x l),mem''') 
		)
		
    | CALLV (f,explst) ->
		let makeEnvMemoryV ((arglist,fenv,mem,outenv) : (id list*env*memory*env)) (expr : exp) : (id list * env * memory * env) =
			match arglist with
			| [] -> raise (Error "InvalidArg")
			| hd::tl -> 
			    let (v,mem') = eval mem outenv expr in
				let (l,mem'') = Mem.alloc mem' in 
				(tl,(Env.bind fenv hd (Addr l)),(Mem.store mem'' l v),outenv)
		in 
		let (arglist,funexp,funenv)=(lookup_env_proc env f) in
		let (remainlst,ffunenv,fmem,_) = List.fold_left makeEnvMemoryV (arglist,funenv,mem,env) explst in 
		(match remainlst with
		| [] -> eval fmem (Env.bind ffunenv f (Proc (arglist,funexp,funenv))) funexp  
		| _ -> raise (Error "InvalidArg")
		)
	| CALLR (f,idlst) ->
		let makeEnvMemoryR ((arglist,fenv,mem,outenv) : (id list*env*memory*env)) (myid : id) : (id list * env * memory * env) =
			match arglist with
			| [] -> raise (Error "InvalidArg")
			| hd::tl -> 
				let l = lookup_env_loc outenv myid in
				(tl,(Env.bind fenv hd (Addr l)),mem,outenv)
		in 
		let (arglist,funexp,funenv)=(lookup_env_proc env f) in
		let (remainlst,ffunenv,fmem,_) = List.fold_left makeEnvMemoryR (arglist,funenv,mem,env) idlst in 
		(match remainlst with
		| [] -> eval fmem (Env.bind ffunenv f (Proc (arglist,funexp,funenv))) funexp  
		| _ -> raise (Error "InvalidArg")
		)
	| TRUE -> (Bool true,mem)
	| FALSE -> (Bool false,mem)
	| NUM n -> (Num n,mem)
	| UNIT -> (Unit,mem)
	| VAR x ->
	  let l = lookup_env_loc env x in
	  let v = Mem.load mem l in
	  (v, mem)
	| ADD (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (Num (a+b),mem'')
	  | _ -> raise (Error "TypeError : add should be get int")
	  )
	| SUB (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (Num (a-b),mem'')
	  | _ -> raise (Error "TypeError : sub should be get int")
	  )
	| MUL (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (Num (a*b),mem'')
	  | _ -> raise (Error "TypeError : mul should be get int")
	  )
	| DIV (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (if (b == 0) then raise (Error "Divide by zero!!")
                           else	(Num (a/b),mem''))
	  | _ -> raise (Error "TypeError : div should be get int")
	  )
	| EQUAL (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (if (a == b) then (Bool true,mem'')
						  else(Bool false,mem''))
	  | (Bool a,Bool b) -> (if (a == b) then (Bool true,mem'')
						  else(Bool false,mem''))
      | (Unit,Unit) -> (Bool true,mem'')						  
	  | _ -> (Bool false,mem'')
	  )
	| LESS (fstexp,sndexp) ->
	  let (fst,mem') = eval mem env fstexp in
	  let (snd,mem'') = eval mem' env sndexp in
	  (match (fst,snd) with
	  | (Num a,Num b) -> (if (a<b) then (Bool true,mem'') else (Bool false,mem''))
	  | _ -> raise (Error "TypeError : less should be get int")
	  ) 
	| NOT inexp ->
		let (resval,mem') = eval mem env inexp in
		(match resval with
		| Bool b -> (Bool (not b),mem')
		| _ -> raise (Error "TypeError : not should be get boolean")
		)
	| SEQ (fstexp,sndexp) ->
	  let (_,mem') = eval mem env fstexp in
	  (eval mem' env sndexp)
	| IF (condexp,truexp,falsexp) ->
	  let (cond,mem') = eval mem env condexp in
	  (match cond with
	  | Bool b -> (if (b) then eval mem' env truexp else eval mem' env falsexp) 
	  | _ ->  raise (Error "TypeError : if condition should be get boolean")
	  )
	| WHILE (condexp,loopexp) ->
	  let (cond,mem') = eval mem env condexp in
	  (match cond with
	  | Bool b -> (if (b) 
					then 
						let (_,mem'') = eval mem' env loopexp in
						(eval mem'' env (WHILE (condexp,loopexp)))
					else (Unit,mem')
				   ) 
	  | _ ->  raise (Error "TypeError : while condition should be get boolean")
	  )

  let run (mem, env, pgm) = 
    let (v, _ ) = eval mem env pgm in
    v
end
