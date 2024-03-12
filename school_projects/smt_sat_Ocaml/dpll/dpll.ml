open List

(* fonctions utilitaires *********************************************)
(* filter_map : ('a -> 'b option) -> 'a list -> 'b list
   disponible depuis la version 4.08.0 de OCaml dans le module List :
   pour chaque élément de `list', appliquer `filter' :
   - si le résultat est `Some e', ajouter `e' au résultat ;
   - si le résultat est `None', ne rien ajouter au résultat.
   Attention, cette implémentation inverse l'ordre de la liste *)
let filter_map filter list =
  let rec aux list ret =
    match list with
    | []   -> ret
    | h::t -> match (filter h) with
      | None   -> aux t ret
      | Some e -> aux t (e::ret)
  in aux list []

(* print_modele : int list option -> unit
   affichage du résultat *)
let print_modele: int list option -> unit = function
  | None   -> print_string "UNSAT\n"
  | Some modele -> print_string "SAT\n";
      let modele2 = sort (fun i j -> (abs i) - (abs j)) modele in
      List.iter (fun i -> print_int i; print_string " ") modele2;
      print_string "0\n"

(* ensembles de clauses de test *)
let exemple_3_12 = [[1;2;-3];[2;3];[-1;-2;3];[-1;-3];[1;-2]]
let exemple_7_2 = [[1;-1;-3];[-2;3];[-2]]
let exemple_7_4 = [[1;2;3];[-1;2;3];[3];[1;-2;-3];[-1;-2;-3];[-3]]
let exemple_7_8 = [[1;-2;3];[1;-3];[2;3];[1;-2]]
let systeme = [[-1;2];[1;-2];[1;-3];[1;2;3];[-1;-2]]
let coloriage = [[1;2;3];[4;5;6];[7;8;9];[10;11;12];[13;14;15];[16;17;18];[19;20;21];[-1;-2];[-1;-3];[-2;-3];[-4;-5];[-4;-6];[-5;-6];[-7;-8];[-7;-9];[-8;-9];[-10;-11];[-10;-12];[-11;-12];[-13;-14];[-13;-15];[-14;-15];[-16;-17];[-16;-18];[-17;-18];[-19;-20];[-19;-21];[-20;-21];[-1;-4];[-2;-5];[-3;-6];[-1;-7];[-2;-8];[-3;-9];[-4;-7];[-5;-8];[-6;-9];[-4;-10];[-5;-11];[-6;-12];[-7;-10];[-8;-11];[-9;-12];[-7;-13];[-8;-14];[-9;-15];[-7;-16];[-8;-17];[-9;-18];[-10;-13];[-11;-14];[-12;-15];[-13;-16];[-14;-17];[-15;-18]]

(********************************************************************)

(*contient : int -> int list -> bool
  renvoie true si la liste contient l'element x
   *)
   
let rec contient x e = match e with 
    | a::z -> if (a = x) then true else (contient x z )   
    | []  -> false;;
    
    

(*sampleTrue : int -> int list list -> int list list 
   applique la simplification de l'ensemble des clauses en retirant les clauses contenant x *)
   
let rec sampleTrue x l =
  match l with
  | []-> []
  | a::z -> if contient x a then sampleTrue x z else (a::sampleTrue x z)
;;

(*sampleFalse : int -> int list -> int list list 
   applique la simplification d'une clause en retirant le litteral (non x) de celle ci *)
  
let rec sampleFalse x l =
  match l with
  | []-> []
  | a::z -> if a= -x then sampleFalse x z else (a::sampleFalse x z)
;;


(* simplifie : int -> int list list -> int list list 
   applique la simplification de l'ensemble des clauses en mettant
   le littéral l à vrai *)
let rec simplifie l clauses =
  sampleTrue l (List.map (sampleFalse l) clauses) ;;
;;

(* solveur_split : int list list -> int list -> int list option
   exemple d'utilisation de `simplifie' *)
(* cette fonction ne doit pas être modifiée, sauf si vous changez 
   le type de la fonction simplifie *)
let rec solveur_split clauses interpretation =
  (* l'ensemble vide de clauses est satisfiable *)
  if clauses = [] then Some interpretation else
  (* un clause vide n'est jamais satisfiable *)
  if mem [] clauses then None else
  (* branchement *) 
    let l = hd (hd clauses) in
    let branche = solveur_split (simplifie l clauses) (l::interpretation) in
    match branche with
    | None -> solveur_split (simplifie (-l) clauses) ((-l)::interpretation)
    | _    -> branche

(* tests *)
(*let () = print_modele (solveur_split systeme []) *)
(* let () = print_modele (solveur_split coloriage []) *)

(* solveur dpll récursif *)
    
(* unitaire : int list list -> int
    - si `clauses' contient au moins une clause unitaire, retourne
      le littéral de cette clause unitaire ;
    - sinon, lève une exception `Not_found' *)
let rec unitaire clauses =
  match clauses with 
  |[]-> raise Not_found
  |a::z-> if length(a)>1 then unitaire z else List.hd a
  
  
  (*list_of_props : int list list -> int list  
   fusionne toutes les sous listes et renvoie une seule liste *)
let rec list_of_props clauses=
  let rec list l=
    match l with
    |[]-> []
    |a::z -> [a]@list z
  in
  match clauses with
  |[]-> []
  |x::y -> list x @ list_of_props y
;;

(*rech : int -> int list -> int 
   verifie si l'element x est unitaire dans la liste et renvoie 0 sinon *)
let rec rech l x=
  match l with 
  |[]->x
  |a::z -> if a=((-1)*x) then 0 else rech z x
;;

(*recherche_pur : int list -> int -> int 
   fait la recherche dun litteral pur a partir du s ieme element de la liste *)
let rec recherche_pur l s= 
  match l with 
  |[]->failwith"pas de litteral pur"
  |a::z-> if rech l (List.nth l s)!=0 then List.nth l s else 
      if s=( List.length l)-1 then failwith"pas de litteral pur" else recherche_pur l (s+1) 
      
;;


(* pur <: int list list -> int
    - si `clauses' contient au moins un littéral pur, retourne
      ce littéral ;
    - sinon, lève une exception `Failure "pas de littéral pur"' *)
let pur clauses =
  match clauses with 
  |[] -> failwith"pas de litteral pur";
  | a::z -> recherche_pur (list_of_props clauses) 0 ;;



(* solveur_dpll_rec : int list list -> int list -> int list option *)
let rec solveur_dpll_rec clauses interpretation =
  (* l'ensemble vide de clauses est satisfiable *)
  if clauses = [] then Some interpretation else
  (* un clause vide n'est jamais satisfiable *)
  if mem [] clauses then None else
    (* il y a une clause unitaire*)
    try let uni= unitaire clauses in solveur_dpll_rec ( simplifie uni clauses) (uni::interpretation) with 
    |Not_found -> 
    (*il y a un litteral pur*)
      try let purl= pur clauses in solveur_dpll_rec ( simplifie purl clauses)(purl::interpretation) with 
        |Failure _ -> 
          (* branchement *) 
            let l = hd (hd clauses) in
            let branche = solveur_dpll_rec (simplifie l clauses) (l::interpretation) in
            match branche with
            | None -> solveur_dpll_rec (simplifie (-l) clauses) ((-l)::interpretation)
            | _    -> branche
        ;;

(* tests 
let () = print_modele (solveur_split exemple_7_2 []) 
let () = print_modele (solveur_dpll_rec exemple_7_2 []) 
*)

let () =
  let clauses = Dimacs.parse Sys.argv.(1) in
  print_modele (solveur_dpll_rec clauses [])
