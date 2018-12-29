(* malloc *)
(*
let val l = malloc (fn x=> (1 + x)) in
  (!l) 2
end
let val a = malloc (fn x=> (1+x)) in a
end
let val a = malloc (fn x => (1+x)) in a
end

let val a = malloc (fn x => (fn y => (x+y))) in a
end

let val a = malloc (fn x => (fn y => (x and y))) in !a
end
*)

let val a = malloc true
    val b = malloc 2 in
(a=b)
end 


(* result : int *)
