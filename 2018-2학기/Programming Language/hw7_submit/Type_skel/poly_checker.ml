(*
 * SNU 4190.310 Programming Languages
 * Type Checker Skeleton
 *)

open M

type var = string

type typ = 
  | TInt
  | TBool
  | TString
  | TPair of typ * typ
  | TLoc of typ
  | TFun of typ * typ
  | TVar of var
  | TCompare of var
  | TWrite of var
  (* Modify, or add more if needed *)

(* Tvar > Tcompare > Twrite. Tvar는 아무거나, Tcompare는 fun,pair등 제외, Twrite는 int,bool,string 빼고 안됨.*)
type typ_scheme =
  | SimpleTyp of typ 
  | GenTyp of (var list * typ)

type typ_env = (M.id * typ_scheme) list

let count = ref 0 

let new_var () = 
  let _ = count := !count +1 in
  "x_" ^ (string_of_int !count)

let new_name () = 
  let _ = count := !count + 1 in
  "x_" ^ (string_of_int !count)  
  
let (@+) f (x, v) = (fun y -> if y = x then v else f y)
  
  
(* Definitions related to free type variable *)
(* ftv : var list. free variable list임. *)
let union_ftv ftv_1 ftv_2 = (* ftv 합칩 *)
  let ftv_1' = List.filter (fun v -> not (List.mem v ftv_2)) ftv_1 in
  ftv_1' @ ftv_2

let sub_ftv ftv_1 ftv_2 = (* ftv 차집합 *)
  List.filter (fun v -> not (List.mem v ftv_2)) ftv_1

let rec ftv_of_typ : typ -> var list = function
  | TInt | TBool | TString -> []
  | TPair (t1, t2) -> union_ftv (ftv_of_typ t1) (ftv_of_typ t2)
  | TLoc t -> ftv_of_typ t
  | TFun (t1, t2) ->  union_ftv (ftv_of_typ t1) (ftv_of_typ t2)
  | TVar v | TCompare v | TWrite v -> [v]

let ftv_of_scheme : typ_scheme -> var list = function
  | SimpleTyp t -> ftv_of_typ t
  | GenTyp (alphas, t) -> sub_ftv (ftv_of_typ t) alphas 

(* env : M.id * typ_scheme list *) 
let ftv_of_env : typ_env -> var list = fun tyenv ->
  List.fold_left 
    (fun acc_ftv (id, tyscm) -> union_ftv acc_ftv (ftv_of_scheme tyscm))
    [] tyenv 

(* Generalize given type into a type scheme *)
let generalize : typ_env -> typ -> typ_scheme = fun tyenv t ->
  let env_ftv = ftv_of_env tyenv in
  let typ_ftv = ftv_of_typ t in
  let ftv = sub_ftv typ_ftv env_ftv in
  if List.length ftv = 0 then
    SimpleTyp t
  else
    GenTyp(ftv, t)

(* Definitions related to substitution *)

type subst = typ -> typ

let empty_subst : subst = fun t -> t

let make_subst : var -> typ -> subst = fun x t ->
  let rec subs t' = 
    match t' with
    | TVar x' -> if (x = x') then t else t'
    | TPair (t1, t2) -> TPair (subs t1, subs t2)
    | TLoc t'' -> TLoc (subs t'')
    | TFun (t1, t2) -> TFun (subs t1, subs t2)
    | TInt | TBool | TString -> t'
	| TCompare x' -> if (x = x') then (* type compression. *)
					 (match t with
					 | TVar x'' -> TCompare x''
					 | _ -> t
					 )
					 else t'
	| TWrite x' -> if (x = x') then (* type compression *)
					 (match t with
					 | TVar x'' -> TWrite x''
					 | TCompare x'' -> TWrite x''
					 | _ -> t
					 )
				   else t'
  in subs

let (@@) s1 s2 = (fun t -> s1 (s2 t)) (* substitution composition *)

let subst_scheme : subst -> typ_scheme -> typ_scheme = fun subs tyscm ->
  match tyscm with
  | SimpleTyp t -> SimpleTyp (subs t)
  | GenTyp (alphas, t) ->
    (* S (\all a.t) = \all b.S{a->b}t  (where b is new variable) *)
    let betas = List.map (fun _ -> new_var()) alphas in
    let s' =
      List.fold_left2
        (fun acc_subst alpha beta -> make_subst alpha (TVar beta) @@ acc_subst)
        empty_subst alphas betas
    in
    GenTyp (betas, subs (s' t))

let subst_env : subst -> typ_env -> typ_env = fun subs tyenv ->
  List.map (fun (x, tyscm) -> (x, subst_scheme subs tyscm)) tyenv

let rec hasSameType (a:typ) (b:typ) : bool = 
	match (a,b) with
	| (TInt,TInt) -> true
	| (TBool,TBool) -> true
	| (TString,TString) -> true
	| (TLoc ta,TLoc tb) -> (hasSameType ta tb)
	| (TPair (a1,a2),TPair (b1,b2)) -> ((hasSameType a1 b1) && (hasSameType a2 b2))
	| (TFun (a1,a2),TFun (b1,b2)) -> ((hasSameType a1 b1) && (hasSameType a2 b2))
	| (TVar sa,TVar sb) | (TCompare sa,TCompare sb) | (TWrite sa, TWrite sb) -> (if (sa=sb) then true else false)
	| _ -> false 	

let rec unify (ty1 : typ) (ty2 : typ) : subst =
	let rec typehasVar (x : var) (ty : typ) : bool =
		(match ty with
		| TInt -> false
		| TBool -> false
		| TString -> false
		| TPair (a,b) -> (typehasVar x a) || (typehasVar x b)
		| TLoc l -> (typehasVar x l)
		| TFun (a,b) -> (typehasVar x a) || (typehasVar x b)
		| TVar v | TCompare v | TWrite v -> (v=x)
		)
	in 
    (match (ty1,ty2) with
	| (TVar x, t) | (t, TVar x) ->
		if (hasSameType (TVar x) t) then empty_subst
		else (if (not (typehasVar x t)) then (make_subst x t)
			  else raise (M.TypeError "There is no matched type"))
	| (TCompare x, t) | (t, TCompare x) ->
		if (hasSameType (TCompare x) t) then empty_subst
		else (if (not (typehasVar x t)) then 
			      (match t with
				  | TInt | TBool | TString | TLoc _ | TCompare _ | TWrite _ -> (make_subst x t)
				  | TPair _ | TFun _ | TVar _ -> raise (M.TypeError "try to compare wrong type")
				  )
			  else raise (M.TypeError "There is no matched type"))
	| (TWrite x, t) | (t, TWrite x)->
		if (hasSameType (TWrite x) t) then empty_subst
		else (if (not (typehasVar x t)) then
				  (match t with
				  | TInt | TBool | TString | TWrite _ -> (make_subst x t)
				  | TLoc _ | TCompare _ | TPair _ | TFun _ | TVar _ -> raise (M.TypeError "try to write wrong type")
				  )
			  else raise (M.TypeError "There is no matched type"))		
	| (TFun (a1,a2), TFun (b1,b2)) ->
		let f1 = (unify a1 b1) in
		let f2 = (unify (f1 a2) (f1 b2)) in
		(f2 @@ f1)
	| (TPair (a1,a2), TPair(b1,b2)) -> 
		let u1 = (unify a1 b1) in
		let u2 = (unify (u1 a2) (u1 b2)) in
		(u2 @@ u1)
	| (TLoc a, TLoc b) -> (unify a b)
	| (a,b) ->
		if (hasSameType a b) then empty_subst
		else raise (M.TypeError "There is no matched type")
	)  

let rec expansive (exp: M.exp): bool =
  match exp with
	| M.CONST _ -> false
	| M.VAR x -> false
	| M.FN (x,e) -> false
	| M.APP (e1,e2) -> true
	| M.LET (decl, e) ->
		(match decl with
		| M.VAL (x_id,x_exp) -> (expansive x_exp || expansive e)
		| M.REC (f_id,a_id,f_exp) -> (expansive e)		(* f_exp는 무조건 function type *)
		)
	| M.IF (e1, e2, e3) -> 
		(expansive e1 || expansive e2 || expansive e3)
	| M.READ -> false
	| M.WRITE e -> expansive e
	| M.MALLOC e -> true
	| M.ASSIGN (e1,e2) -> (expansive e1 || expansive e2)
	| M.BANG e -> (expansive e)
	| M.SEQ (e1,e2) -> (expansive e1 || expansive e2)
	| M.BOP (op, e1, e2) -> (expansive e1 || expansive e2)
	| M.PAIR (e1, e2) -> (expansive e1 || expansive e2)
	| M.FST e -> (expansive e)
	| M.SND e -> (expansive e)

(*
type typ = 
  | TInt
  | TBool
  | TString
  | TPair of typ * typ
  | TLoc of typ
  | TFun of typ * typ
  | TVar of var
  
*)


let rec malgol (env:typ_env) (exp:M.exp) (t: typ) : subst = 
	match exp with
	| M.CONST (M.S s) -> (unify t TString)
	| M.CONST (M.N n) -> (unify t TInt) 
	| M.CONST (M.B b) -> (unify t TBool)
	| M.VAR x -> 
		let typesch = try List.assoc x env with Not_found -> raise (M.TypeError ("type not matched")) in
		(match typesch with
		| SimpleTyp typ -> (unify t typ)
		| GenTyp (alphas, typ) -> 
			let scheme = (subst_scheme empty_subst typesch) in 
			(match scheme with 
			| GenTyp (alp,ty) -> (unify t ty)
			| _ -> raise (M.TypeError ("Something Terrible occured!"))
			)
		)
	| M.FN (x,e) -> 
		let n1 = new_var() in
		let n2 = new_var() in 
		let t1 = (TVar n1) in
		let t2 = (TVar n2) in
		let s1 = (unify t (TFun (t1,t2)) ) in
		let env' = subst_env s1 env in
		let env'' = [x,SimpleTyp (s1 t1)] @ env' in
		let s2 = (malgol env'' e (s1 t2)) in
		s2 @@ s1
	| M.APP (e1,e2) ->
		let n1 = new_var() in 
		let t1 = (TVar n1) in 
		let s1 = (malgol env e1 (TFun (t1,t)) ) in
		let env' = subst_env s1 env in
		let s2 = (malgol env' e2 (s1 t1)) in
		s2 @@ s1
	| M.LET (decl, e) ->
		(match decl with
		| M.VAL (x_id,x_exp) -> 
			let n1 = new_var() in 
			let t1 = (TVar n1) in 
			let s1 = (malgol env x_exp t1) in
			let env' = subst_env s1 env in (* s1 gamma *)
			let t1' = (s1 t1) in 
			let t' = (s1 t) in 
			let genedx = (if (expansive x_exp) then (SimpleTyp t1') (*check expansive *)
                       	  else (generalize env' t1')) in 
			let env'' = [x_id,genedx] @ env' in
			let s2 = (malgol env'' e t') in 
			s2 @@ s1
		| M.REC (f_id,a_id,f_exp) ->
			let n1 = new_var() in 
			let t1 = (TVar n1) in
			let s1 = (malgol ([f_id, SimpleTyp t1] @ env) (M.FN (a_id,f_exp)) t1) in
			let t1' = (s1 t1) in 
			let t' = (s1 t) in
			let env' = (subst_env s1 env) in 
			let genedf = (generalize env' t1') in (* f_exp는 무조건 function type *)
			let env'' = [f_id,genedf] @ env' in 
			let s2 = (malgol env'' e t') in 
			s2 @@ s1
		)
	| M.IF (e1, e2, e3) -> 
		let s1 = (malgol env e1 TBool) in
		let env' = (subst_env s1 env) in
		let t' = (s1 t) in
		let s2 = (malgol env' e2 t') in
		let env'' = (subst_env s2 env') in
		let t'' = (s2 t') in
		let s3 = (malgol env'' e3 t'') in 
		s3 @@ s2 @@ s1
    | M.READ -> (unify t TInt)
    | M.WRITE e -> 
		let n1 = new_var() in 
		let t1 = (TWrite n1) in 
		let s1 = (unify t t1) in
		let t' = (s1 t) in
		let env' = (subst_env s1 env) in 
		let s2 = (malgol env' e t') in
		s2 @@ s1
    | M.MALLOC e -> 
		let n1 = new_var() in
		let t1 = (TVar n1) in
		let s1 = (malgol env e t1) in
		let t1' = (s1 t1) in
		let t' = (s1 t) in
		let s2 = (unify t' (TLoc t1')) in
		s2 @@ s1
    | M.ASSIGN (e1,e2) ->
		let s1 = (malgol env e1 (TLoc t)) in
		let t' = (s1 t) in 
		let env' = (subst_env s1 env) in
		let s2 = (malgol env' e2 t') in
		s2 @@ s1
    | M.BANG e -> (malgol env e (TLoc t))
    | M.SEQ (e1,e2) ->
		let n1 = new_var() in 
		let t1 = (TVar n1) in 
		let s1 = (malgol env e1 t1) in
		let t' = (s1 t) in
		let env' = (subst_env s1 env) in 
		let s2 = (malgol env' e2 t') in 
		s2 @@ s1
    | M.BOP (op, e1, e2) -> 
		(match op with
		 | M.ADD | M.SUB -> 
			let s1 = (unify t TInt) in
			let env' = (subst_env s1 env) in
			let s2 = (malgol env' e1 TInt) in
			let env'' = (subst_env s2 env') in
			let s3 = (malgol env'' e2 TInt) in
			s3 @@ s2 @@ s1
		 | M.AND | M.OR -> 
			let s1 = (unify t TBool) in
			let env' = (subst_env s1 env) in
			let s2 = (malgol env' e1 TBool) in
			let env'' = (subst_env s2 env') in
			let s3 = (malgol env'' e2 TBool) in
			s3 @@ s2 @@ s1
		 | M.EQ -> 
			let n1 = new_var() in 
			let t1 = (TCompare n1) in 
			let s1 = (unify t TBool) in
			let env' = (subst_env s1 env) in
			let t1' = (s1 t1) in 
			let s2 = (malgol env' e1 t1') in
			let env'' = (subst_env s2 env') in 
			let t1'' = (s2 t1') in 
			let s3 = (malgol env'' e2 t1'') in
			s3 @@ s2 @@ s1
		 )
    | M.PAIR (e1, e2) -> 
		let n1 = new_var() in
		let n2 = new_var() in 
		let t1 = (TVar n1) in 
		let t2 = (TVar n2) in 
		let s1 = (unify t (TPair (t1,t2))) in 
		let t1' = (s1 t1) in
		let t2' = (s1 t2) in 
		let env' = (subst_env s1 env) in 
		let s2 = (malgol env' e1 t1') in
		let t2'' = (s2 t2') in
		let env'' = (subst_env s2 env') in 
		let s3 = (malgol env'' e2 t2'') in
		s3 @@ s2 @@ s1
    | M.FST e ->
		let n1 = new_var() in 
		let t1 = (TVar n1) in 
		(malgol env e (TPair (t,t1)))
    | M.SND e -> 
		let n1 = new_var() in
		let t1 = (TVar n1) in
		(malgol env e (TPair (t1,t)))


let rec typeconvert (t : typ) : M.typ =
	match t with
	| TInt -> M.TyInt
	| TBool -> M.TyBool
	| TString -> M.TyString
	| TPair (a,b) -> M.TyPair (typeconvert a,typeconvert b)
	| TLoc l -> (M.TyLoc (typeconvert l))
	| _ -> raise (M.TypeError "converting failed!")

(* TODO : Implement this function *)
let check : M.exp -> M.typ = fun input -> 
	let n1 = new_var() in
	let t1 = (TVar n1) in 
	let sub = (malgol [] input t1) in
	(typeconvert (sub t1))

