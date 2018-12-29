(* exercise 1-3 *)

type team = Korea | France | Usa | Brazil | Japan | Nigeria | Cameroon
| Poland | Portugal | Italy | Germany | Norway | Sweden | England
| Argentina

type tourna = LEAF of team
| NODE of tourna * tourna

let rec parenize (tor : tourna) : string = 
let team_to_string (tem : team) : string = match tem with
|Korea -> "Korea"
|France -> "France"
|Usa -> "Usa"
|Brazil -> "Brazil"
|Japan -> "Japan"
|Nigeria -> "Nigeria"
|Cameroon -> "Cameroon"
|Poland  -> "Poland"
|Portugal -> "Portugal"
|Italy -> "Italy"
|Germany -> "Germany"
|Norway -> "Norway"
|Sweden -> "Sweden"
|England -> "England"
|Argentina -> "Argentina" in
match tor with
	|LEAF t -> team_to_string t
	|NODE (a,b) -> "("^(parenize a)^" "^(parenize b)^")"