open Printf

(* Définitions de terme, test et programme *)
type term = 
 | Const of int
 | Var of int
 | Add of term * term
 | Mult of term * term

type test = 
 | Equals of term * term
 | LessThan of term * term

let tt = Equals (Const 0, Const 0)
let ff = LessThan (Const 0, Const 0)
 
type program = {nvars : int; 
                inits : term list; 
                mods : term list; 
                loopcond : test; 
                assertion : test}

let x n = "x" ^ string_of_int n

(* Question 1. Écrire des fonctions `str_of_term : term -> string` 
   et `str_of_test : test -> string` qui convertissent des termes 
   et des tests en chaînes de caractères du format SMTLIB.

  Par exemple, str_of_term (Var 3) retourne "x3", str_of_term (Add
   (Var 1, Const 3)) retourne "(+ x1 3)" et str_of_test (Equals (Var
   2, Const 2)) retourne "(= x2 2)".  *)

  (*la fonction str_of_test prend : test -> string 
    Cette fonction parcours le test qu'on lui a donné en argument :
    Si on a un Equals on devra renvoyer le Equals sous forme "=" puis faire appel à la fonction str_of_term avec le terme a et b
    enfin si c'est pas un Equals qu'on a on aura forcement un "<" donc si on tombe sur LessThan on renverra "<" et on ferra 
    appel à la fonction str_of_term avec le terme a et b.
 *)

let rec str_of_term t = match t with
  |Const a -> string_of_int a
  |Var a -> (x a)
  |Add (a,b) -> "(+ " ^ str_of_term a ^ " " ^ str_of_term b ^")"
  |Mult (a,b) -> "(* " ^ str_of_term a ^ " " ^ str_of_term b ^")"
;; 

let str_of_test t = match t with
 |Equals (a,b) -> "(= " ^ str_of_term a ^ " " ^ str_of_term b ^")"
 |LessThan (a,b) -> "(< " ^ str_of_term a ^ " " ^ str_of_term b ^")"
;;

let string_repeat s n =
  Array.fold_left (^) "" (Array.make n s)

(* Question 2. Écrire une fonction `str_condition : term list -> string`
   qui prend une liste de termes t1, ..., tk et retourne une chaîne 
   de caractères qui exprime que le tuple (t1, ..., tk) est dans 
   l'invariant.  Par exemple, str_condition [Var 1; Const 10] retourne 
   "(Inv x1 10)".
   *)

(*La fonction str_condition prend : term list -> string
  Cette fonction contiens une fonction auxiliaire qui est aux.
  La fonction aux permet de parcourir la liste l si celle-ci est vide on renvoie une chaine vide sinon on fait appel 
  à la fonction string_repeat qui nous a était donnée et avec celle-ci on appelera la fonction str_of_term avec l'element a 
  puis on décide de le répeter une seule fois et de le concaténer avec l'appel recusive du reste de la liste.
  Enfin lorsqu'on aura fini de parcourir la liste on renverra "(Invar" avec la reponse que nous a fournie l'appel de aux 
  avec la liste l(donner en argument).
*)

let str_condition l=
  let rec aux l = match l with
    | [] -> ""
    |a::b -> string_repeat (" "^(str_of_term a)) (1)^ aux b
  in "(Invar"^aux l^")"
;;

(* Question 3. Écrire une fonction 
   `str_assert_for_all : int -> string -> string` qui prend en
   argument un entier n et une chaîne de caractères s, et retourne
   l'expression SMTLIB qui correspond à la formule "forall x1 ... xk
   (s)".

  Par exemple, str_assert_forall 2 "< x1 x2" retourne : "(assert
   (forall ((x1 Int) (x2 Int)) (< x1 x2)))".  *)

   (* la fonction str_asser_forall contient une fonction 
      auxiliaire `aux : int -> int -> string -> string -> string`
      qui prend deux entiers (nb et n) et deux chaines de caractères (tmp et s) 
      et retourne une chaine de caractères. nb est un compteur pour savoir lorsqu'on a plus
      de variable donc n (nombre de variable). 
      la fonction verifie donc sil ya des variable ou non s'il y a encore une chainde de caratères on la met 
      a l'intérieure d'une parenthese  avec un Int et fermer la parenthese.
      
      La fonction str_assert_forall fais appelle a la fonction str_assert (string -> string) qui permet de rajouté 
      "(assert s )" avec s un string *)

let str_assert s = "(assert " ^ s ^ ")"

let str_assert_forall n s =
  let rec aux nb n tmp s=
    if nb=n+1 then "("^tmp ^") "^"("^ s^")"
    else aux (nb+1) (n) (tmp^("("^(x nb)^" Int)")) (s)
in str_assert("(forall "^aux(1)(n)("")(s)^")")
;;
  
(* Question 4. Nous donnons ci-dessous une définition possible de la
   fonction smt_lib_of_wa. Complétez-la en écrivant les définitions de
   loop_condition et assertion_condition. *)

   (*`aux : int -> int -> term list  aux qui prend comme argument 2 int i et a 
    et une list de term b qui permet de renvoyer une list de variable 
    
    `loop_condition p: program -> string`la fonction prend en argument un program p et renvoye une chaine de caractères 
    cette fonction permet de renvoyer "(assert(forall(" les variables "(=> (and" les conditions de l'invariant 
    et les conditions des mods 
    
    `assertion_condition p: program -> string` la fonction prend en argument un program p et renvoye une chaine de caractères
    cette fonction permet de renvoyer l'assertion finale qui est vérifiée sous forme de 
    "(assert(forall(" les variables "(=> (and" les conditions de l'invariant et les conditions des assertion 
  *)

  let rec aux i a b = 
    if i = a+1 then b
    else aux (i+1) a (b@[Var i]) 
  ;;

let smtlib_of_wa p = 
  let declare_invariant n =
    "; synthèse d'invariant de programme\n"
    ^"; on déclare le symbole non interprété de relation Invar\n"
    ^"(declare-fun Invar (" ^ string_repeat "Int " n ^  ") Bool)" in
  let loop_condition p =
    "; la relation Invar est un invariant de boucle\n"
    ^str_assert_forall p.nvars ("=> (and" ^" "^str_condition(aux(1)(p.nvars)([]))^
    str_of_test p.loopcond^")"^" "^str_condition p.mods) in
  let initial_condition p =
    "; la relation Invar est vraie initialement\n"
    ^str_assert (str_condition p.inits) in
  let assertion_condition p =
    "; l'assertion finale est vérifiée\n"
    ^str_assert_forall p.nvars ("=> (and" ^" "^str_condition(aux(1)(p.nvars)([]))^
    "(not"^str_of_test p.loopcond^"))"^" "^str_of_test p.assertion) in
  let call_solver =
    "; appel au solveur\n(check-sat-using (then qe smt))\n(get-model)\n(exit)\n" in
  String.concat "\n" [declare_invariant p.nvars;
                      loop_condition p;
                      initial_condition p;
                      assertion_condition p;
                      call_solver]
  ;;                    

let p1 = {nvars = 2;
          inits = [(Const 0) ; (Const 0)];
          mods = [Add ((Var 1), (Const 1)); Add ((Var 2), (Const 3))];
          loopcond = LessThan ((Var 1),(Const 3));
          assertion = Equals ((Var 2),(Const 9))}
;;

let () = Printf.printf "%s" (smtlib_of_wa p1)
;;

(* Question 5. Vérifiez que votre implémentation donne un fichier
   SMTLIB qui est équivalent au fichier que vous avez écrit à la main
   dans l'exercice 1. Ajoutez dans la variable p2 ci-dessous au moins
   un autre programme test, et vérifiez qu'il donne un fichier SMTLIB
   de la forme attendue. *)

   (* p2 traduit le programme suivant pour trouver l'invariant de boucle :

      int x1= 1;
      int x2= 1;
      while(i < 15){
        x1 = x1 +1;
        x2 = x2 +1;
      } 
      assert(x1 = 15)*)

   let p2 = {nvars = 2;
   inits = [(Const 1) ; (Const 1)];
   mods = [Add ((Var 1), (Const 1)); Add ((Var 2), (Const 1))];
   loopcond = LessThan ((Var 1),(Const 15));
   assertion = Equals ((Var 1),(Const 15))}
;;

let () = Printf.printf "%s" (smtlib_of_wa p2);;

