type require = id * (cond list)
and cond = Items of gift list (* 선물들 *)
		| Same of id (* 어느 조카의 선물들 *)
		| Common of cond * cond (* 두조건에 공통된 선물들 *)
		| Except of cond * gift list (* 조건에서 어느 선물들은 빼고 *)
and gift = int (* 선물 번호 *)
and id = A | B | C | D | E (* 조카 이름 *)

(* compare 함수들은 정렬 안되어있으면 무한루프돈다! 주의 !! *)

exception Error of string

type giftset = gift list (*gift set은 정렬된 int 배열 *)

let makeEmptygift (i : id) : (id * giftset) = (i,[])

let rec setAdd (set : giftset) (mem : gift) : giftset =
	match set with
	| [] -> mem::[]
	| hd::tl -> (if (hd==mem) then hd::tl 
				 else (if (hd > mem) then mem::set 
					   else hd::(setAdd tl mem)
					  )
				)

let rec setDel (set : giftset) (mem : gift) : giftset =
	match set with
	| [] -> []
	| hd::tl -> (if (hd==mem) then tl else hd::(setDel tl mem))
	
let rec listTosetinner (lst : int list) (set : giftset) : giftset =
	match lst with
	| [] -> set
	| hd::tl -> (listTosetinner tl (setAdd set hd))

let listToset (lst : int list) : giftset = (listTosetinner lst [])
	
let setTolist (set : giftset) : int list = set

let rec setUnion (aset : giftset) (bset : giftset) : giftset =
	match bset with
	| [] -> aset
	| hd::tl -> (setUnion (setAdd aset hd) tl)

let rec setMinus (aset : giftset) (bset : giftset) : giftset = (* aset - bset *)
	match bset with
	| [] -> aset
	| hd::tl -> (setMinus (setDel aset hd) tl)

let rec setIntersectinner (aset : giftset) (bset : giftset) (inter : giftset) : giftset = 
	match aset with 
	| [] -> inter
	| ahd::atl -> (match bset with
					| [] -> inter
					| bhd::btl -> (if (ahd==bhd) then (setIntersectinner atl btl (setAdd inter ahd))
								   else (if (ahd>bhd) then (setIntersectinner aset btl inter)
										 else (setIntersectinner atl bset inter)
										 )
					
								   )
				   )
				   
let rec setIntersect (aset : giftset) (bset : giftset) : giftset = 
	(setIntersectinner aset bset [])
	
let rec compareSet (aset : giftset) (bset : giftset) : bool =
	match aset with
	| [] -> (match bset with
			| [] -> true
			| _ -> false)
	| ahd::atl -> (match bset with
				   | [] -> false
				   | bhd::btl -> (if (ahd==bhd) then (compareSet atl btl) else false))


let rec comparegift (a : gift list) (b : gift list) : bool =
	match a with
	| [] -> (match b with
			| [] -> true
			| _ -> false)
	| ahd::atl -> (match b with
				   | [] -> false
				   | bhd::btl -> (if (ahd==bhd) then (comparegift atl btl) else false)
				  )


let rec compareIdgift ((aid,agift) : (id * giftset)) ((bid,bgift) : (id * giftset)) : bool =
	match (aid,bid) with 
	| (A,A) -> (compareSet agift bgift)
	| (B,B)	-> (compareSet agift bgift)
	| (C,C) -> (compareSet agift bgift)
	| (D,D) -> (compareSet agift bgift)
	| (E,E) -> (compareSet agift bgift)
	| _ -> false
	

let rec compareShoplist (a : (id * giftset) list) (b : (id * giftset) list) : bool =
	match a with
	| [] -> (match b with
			| [] -> true
			| _ -> false)
	| ahd::atl -> (match b with
				   | [] -> false
				   | bhd::btl -> ((compareIdgift ahd bhd) && (compareShoplist atl btl))
				  )

let rec getcond (i : id) (req : require list) : cond list = match req with
	| [] -> []
	| (ids,cond)::tl -> (if (ids == i) then cond else (getcond i tl))

let rec getgiftset (i : id) (idgiftlst : (id * giftset) list) : giftset = 
	match idgiftlst with
	| [] -> raise (Error "Error : getgiftlist SomeThing wrong")
	| (ids,idgift)::tl -> (if (ids == i) then idgift else (getgiftset i tl))


let rec getwannabe (i : id) (con : cond) (before : (id * giftset) list) : giftset = 
	match con with
	| Items gl -> (listToset gl)									(* item list 고려해서 sorting 해서 들여오자 *)
	| Same other ->	(getgiftset other before)						(* 다른놈이 갖고있는 itemlist 그대로 갖고오면 된다 *)
	| Common (acon,bcon) -> (setIntersect (getwannabe i acon before) (getwannabe i bcon before)) 				(* a랑 b 컨디션 고려해서, 두 결과로 만들어진 리스트의 교집합을 구하자. *)
	| Except (acon,gl) -> (setMinus (getwannabe i acon before) (listToset gl))                      (* a condition으로 따온 리스트에서 gl에 해당하는 리스트의 멤버들을 없애버리자. *)

	
let rec getwannabeSet (i : id) (conlst : cond list) (before : (id * giftset) list) : giftset =	 (* 애들이 전에 받은걸 보고 id가 원하는것을 뽑아옴 *)
	match conlst with
	| [] -> []
	| hd::tl -> (setUnion (getwannabe i hd before) (getwannabeSet i tl before))

(*	
let addgiftlist (before : (id * giftset) list) (add : (id * giftset) list) : (id * giftset) list = (* 찾은 선물들을 추가함. 중복 고려해서 제거해버림. 추가할때는 정렬을 위해 오름차순으로 한다. *)
	match (before,add) with
	| (bhd::btl,ahd::atl) -> 
*)

	
let findshop (req : require list) (before : (id * giftset) list) : (id * giftset) list = (* 한번 돌리기 *)
    let agift : giftset= (getwannabeSet A (getcond A req) before) in
	let bgift : giftset= (getwannabeSet B (getcond B req) before) in
	let cgift : giftset= (getwannabeSet C (getcond C req) before) in
	let dgift : giftset= (getwannabeSet D (getcond D req) before) in
	let egift : giftset= (getwannabeSet E (getcond E req) before) in
	let apair : (id * giftset) = (A,agift) in 
	let bpair : (id * giftset) = (B,bgift) in 
	let cpair : (id * giftset) = (C,cgift) in 
	let dpair : (id * giftset) = (D,dgift) in 
	let epair : (id * giftset) = (E,egift) in 
	(apair::bpair::cpair::dpair::epair::[])
	(*
	(addgiftlist before ((A,Agift)::(B,Bgift)::(C,Cgift)::(D,Dgift)::(E,Egift)))
	*)
let rec findshoplist (req : require list) (before : (id * giftset) list) : (id * giftset) list = 
	let after = (findshop req before) in
	if (compareShoplist before after) then after else (findshoplist req after)

let getOrderId (a : id) (b : id) : bool =  (* if a is less then b, true. A가 먼저와야하면 트루. *)
	match (a,b) with
	| (A,B) -> true
	| (A,C) -> true
	| (A,D) -> true
	| (A,E) -> true
	| (B,C) -> true
	| (B,D) -> true
	| (B,E) -> true
	| (C,D) -> true
	| (C,E) -> true
	| (D,E) -> true
	| _ -> false

	
let rec sortReqfold (a : require list) ((bi,bc) : require) : require list =
	match a with
	| [] -> (bi,bc)::[]
	| (ai,ac)::tl -> (if (getOrderId bi ai) then (bi,bc)::a
					else (ai,ac)::(sortReqfold tl (bi,bc))
					)
	
let sortReq (req : require list) : require list = (List.fold_left sortReqfold [] req)
	
let shoppingList (req : require list) : (id * gift list) list = 
	(findshoplist (sortReq req)  ((makeEmptygift A)::(makeEmptygift B)::(makeEmptygift C)::(makeEmptygift D)::(makeEmptygift E)::[]))

