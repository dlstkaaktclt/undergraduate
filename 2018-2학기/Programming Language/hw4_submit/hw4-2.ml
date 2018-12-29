type treasure = StarBox | NameBox of string
type key = Bar | Node of key * key
type map = End of treasure
		 | Branch of map * map
		 | Guide of string * map
exception IMPOSSIBLE
(*
type exkey = Bar | Str of string | Node of exkey * exkey
type shape = suggest * cond										(*현재 지도가 담고있는 모양이랑 조건*)
and suggest = (Bar | Str of string | Node of shape * shape)		 (*현재 지도의 모양 *)
and cond = (Str of string * exkey) list							(* 조건 : str = 무엇인가? *)
*)

type shape = Bar | Str of string | Node of shape * shape


let rec makeBranch (sh1 : shape) (sh2 : shape) : shape = 
	if
	

let rec makeshape (m : map) : shape = match map with
	| End e -> (e match with
				| StarBox -> (Bar)
				| NameBox s -> (Str s)
	| Branch (map1,map2) -> let shape2 = makeshape(map2) in
							let shape1 = makeshape(map1) in
							(makeBranch shape1 shape2)
							
	| Guide (g,mapin) -> let shapeinner = makeshape(mapin) in
						 Node ((Str g),shapeinner)





		 
(* 결국 일단 지도를 만들수 있다면 해가 존재한다는 소리.
   해가 존재하면 그냥 그 해를 계산하면 됨. 어떻게? 몰라 일단 나중에 생각해요 *)
   
   
   
   
   
   
   
   
   
   
let rec findtreasure (m : map) (s : string) : 

let rec bCouldBeMatched (left : shape) (right : shape) : bool = left match with
	| Bar -> (right match with
				| Bar -> true
				| Str s -> true
				| _ -> false)
	| Str s -> (right match with
				| Bar -> true
				| Str s -> true
				| Node (l,r) -> true)
	| Node (l,r) -> 
   
let rec makeshape (m : map) : shape = match map with
	| End e -> (e match with
				| StarBox -> Bar
				| NameBox s -> (Str s))
	| Branch (map1,map2) -> let shape2 = makeshape(map2) in
							let shape1 = makeshape(map1) in
							(if 
							
	| Guide (g,mapin) ->
	
   

let getReady (m : map) : key list = 