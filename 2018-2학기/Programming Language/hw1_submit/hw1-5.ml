(* exercise 1-5 *)


type nat = ZERO | SUCC of nat

let rec natadd ((a,b):(nat * nat)) : nat =
	match a with
	|ZERO -> b
	|SUCC n -> natadd(n,SUCC b)

let rec natmul ((a,b):(nat * nat)) : nat =
	match a with
	|ZERO -> ZERO
	|SUCC n -> natadd(b,natmul(n,b))

