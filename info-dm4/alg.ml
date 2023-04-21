type alg = Nil | Letter of char * alg * alg * bool
(* Letter, fils, frere, isEnd? *)

let example = Letter ( 'i', Letter ('n', Letter ('f', Letter ('o', Nil, Nil, true), Nil, true), Nil, false), Letter ('m', Letter ('a', Letter ('t', Letter ('e', Letter ('l', Letter ('a', Letter ('s', Nil, Nil, true), Nil, false), Nil, false), Letter ('h', Nil, Letter('r', Letter ('i', Letter ('c', Letter ('e', Nil, Nil, true),Nil, false), Nil, false), Nil, false), true), false),Nil, false), Nil, false), Nil, false), false)

(* Complexité : linéaire en la taille de la chaine de caractères *)
let charlist_of_string str = 
  let rec aux i = 
    if i < 0 then [] 
    else str.[i] :: aux (i-1) 
  in aux (String.length str - 1)

(* Complexité : linéaire en la taille de la chaine de caractères *)
let print_charlist (l: char list) : unit = 
  let rec print_nonewline = function
  | [] -> ()
  | hd :: tl ->print_nonewline tl;  print_char hd
in print_nonewline l; print_newline ()

(* Complexité : constante*)
let print_bool (b: bool) : unit = 
  if b then print_string "true" else print_string "false"

(* Complexité : linéaire en la taille de la chaine de caractères plus la taille
   de la concaténation de tous les mots *)
let print_alg (a: alg) : unit = 
  let rec affiche_acc (a: alg) (curr: char list) : unit = 
   match a with 
  |Nil -> ()
  | Letter (c, fg, fd, e) ->
    affiche_acc fg (c :: curr);
    affiche_acc fd curr;
    if e then print_charlist (c :: curr);
    in affiche_acc a []
  

(* Complexité : linéaire en la longueur du mot car le nombre de visite de `fd`
   pour chaque lettre est majoré par 26*)
let rec add_word (a: alg) (word: char list) : alg =
  match a, word with
  | Nil, hd :: [] -> add_word (Letter (hd, Nil, Nil, true)) []
  | Nil, hd :: tl -> add_word (Letter (hd, Nil, Nil, false)) tl
  | _, [] -> a
  | Letter (c, fg, fd, e), hd :: [] when c = hd -> Letter (c, fg, fd, true)
  | Letter (c, fg, fd, e), hd :: tl when c = hd -> Letter (c, (add_word fg tl), fd, e)
  | Letter (c, fg, fd, e), hd :: tl ->Letter (c, fg, (add_word fd word), e)

(* Complexité : linéaire en la longueur du mot car le nombre de visite de `fd`
   pour chaque lettre est majoré par 26*)
let rec delete_word (a: alg) (word: char list) : alg =
  match a, word with
  | Nil, _ -> Nil
  | _, [] -> a
  | Letter (c, fg, fd, e), hd :: [] when c = hd -> Letter (c, fg, fd, false)
  | Letter (c, fg, fd, e), hd :: tl when c = hd -> Letter (c, (delete_word fg tl), fd, e)
  | Letter (c, fg, fd, e), hd :: tl ->Letter (c, fg, (delete_word fd word), e)