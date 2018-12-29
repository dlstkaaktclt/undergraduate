
type heap = EMPTY 
| NODE of rank * value * heap * heap 
and rank = int and value = int

exception EmptyHeap 

let rank (h : heap) : int = match h with 
  | EMPTY -> -1 
  | NODE(r,_,_,_) -> r 

let shake ((x,lh,rh) : (int * heap * heap)) : heap = 
  if (rank lh) >= (rank rh) 
  then NODE((rank rh)+1, x, lh, rh) 
  else NODE((rank lh)+1, x, rh, lh) 

let findMin (h : heap) : int = match h with 
  | EMPTY -> raise EmptyHeap 
  | NODE(_,x,_,_) -> x 
  
let rec merge ((lh,rh) : (heap*heap)) : heap = match (lh,rh) with
			|(EMPTY,EMPTY) -> EMPTY
			|(EMPTY,r_heap) -> r_heap
			|(l_heap,EMPTY) -> l_heap
			|(l_heap,r_heap) -> if ((findMin l_heap) < (findMin r_heap))
								then (match l_heap with
										| EMPTY -> raise EmptyHeap
										| NODE(_,_,lc,rc) -> (shake (findMin l_heap,merge(lc,rc),r_heap))
									  )
								else (match r_heap with
										| EMPTY -> raise EmptyHeap
										| NODE(_,_,lc,rc) -> (shake (findMin r_heap,l_heap,merge(lc,rc)))
									 )  
  
  
let insert ((x,h) : (int*heap)) : heap = merge(h, NODE(0,x,EMPTY,EMPTY)) 

let deleteMin (h : heap) : heap = match h with 
  | EMPTY -> raise EmptyHeap 
  | NODE(_,x,lh,rh) -> merge (lh,rh)
  
(*
let rec checkheap (h : heap) : bool = match h with
	| EMPTY -> true
	| NODE(r,x,lh,rh) -> (match (lh,rh) with
							|(EMPTY,EMPTY) -> true
							|(NODE(rl,xl,lhl,rhl),EMPTY) -> (checkheap lh)
							|(EMPTY,NODE(rr,xr,lhr,rhr)) -> false
							|(NODE(rl,xl,lhl,rhl),NODE(rr,xr,lhr,rhr)) -> if (rl<rr) then false
																		  else (checkheap lh) && (checkheap rh)
						)

let a = EMPTY						
(*
val a : heap = EMPTY
# checkheap a;;
- : bool = true *)
let a1 = insert (1,a);;
(*
val a1 : heap = NODE (0, 1, EMPTY, EMPTY)
*)
let a2 = insert (2,a1);;
(*
val a2 : heap = NODE (0, 1, NODE (0, 2, EMPTY, EMPTY), EMPTY)
*)
let a3 = insert (3,a2);;
(*
val a3 : heap =
  NODE (1, 1, NODE (0, 2, EMPTY, EMPTY), NODE (0, 3, EMPTY, EMPTY))
*)
let a4 = insert (10,a3);;
(*
val a4 : heap =
  NODE (1, 1, NODE (0, 2, NODE (0, 3, EMPTY, EMPTY), EMPTY),
   NODE (0, 10, EMPTY, EMPTY))
*)
let a5 = insert (15,a4);;
(*
val a5 : heap =
  NODE (1, 1,
   NODE (1, 2, NODE (0, 3, EMPTY, EMPTY), NODE (0, 10, EMPTY, EMPTY)),
   NODE (0, 15, EMPTY, EMPTY))
# checkheap a5;;
- : bool = true
*)
let a6 = insert (4,a5);;
(*
val a6 : heap =
  NODE (1, 1,
   NODE (1, 2, NODE (0, 3, NODE (0, 10, EMPTY, EMPTY), EMPTY),
    NODE (0, 15, EMPTY, EMPTY)),
   NODE (0, 4, EMPTY, EMPTY))
# checkheap a6;;
- : bool = true
*)
let a7 = insert (5,a6);;
(*
val a7 : heap =
  NODE (1, 1,
   NODE (1, 2,
    NODE (1, 3, NODE (0, 10, EMPTY, EMPTY), NODE (0, 15, EMPTY, EMPTY)),
    NODE (0, 4, EMPTY, EMPTY)),
   NODE (0, 5, EMPTY, EMPTY))
*)
let a8 = deleteMin a7;;
(*
val a8 : heap =
  NODE (1, 2,
   NODE (1, 3, NODE (0, 10, NODE (0, 15, EMPTY, EMPTY), EMPTY),
    NODE (0, 4, EMPTY, EMPTY)),
   NODE (0, 5, EMPTY, EMPTY))
# checkheap a8;;
- : bool = true
*)
let a9 = deleteMin a8;;
(*
val a9 : heap =
  NODE (1, 3,
   NODE (0, 4, NODE (0, 10, NODE (0, 15, EMPTY, EMPTY), EMPTY), EMPTY),
   NODE (0, 5, EMPTY, EMPTY))
# checkheap a9;;
- : bool = true
*)

let b = (NODE (1,0,EMPTY,NODE (0,1,EMPTY,EMPTY)));;
(*
val b : heap = NODE (1, 0, EMPTY, NODE (0, 1, EMPTY, EMPTY))
# checkheap b;;
- : bool = false
*)
*)

