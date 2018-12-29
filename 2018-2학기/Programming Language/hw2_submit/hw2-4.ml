module type Queue = 
sig 
	type element
	type queue
	exception EMPTY_Q
	val emptyQ: queue
	val enQ: queue * element -> queue 
	val deQ: queue -> element * queue 
end

module IntListQ = 
struct 
	type element = int list 
	type queue = (element list * element list)
	exception EMPTY_Q 
	let emptyQ : queue= ([],[]) 
	let enQ ((que, elem) : (queue*element)) : queue = match que with
							|(il,ol) -> ((elem::il),ol)
							
	let deQ (que : queue) : (element * queue) = match que with
			| (il,ol) -> (match ol with
							| [] ->  (match (List.rev il) with 
										| [] -> raise EMPTY_Q
										| hd::tl -> (hd,([],tl))
									  )
							| hd::tl -> (hd,(il,tl))
						  )
end

(*
let (n, queue') = IntListQ.deQ (IntListQ.enQ(IntListQ.enQ(IntListQ.emptyQ, [1;2;3]), [4;5]))
*)