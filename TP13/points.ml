(* (abscisse, ordonnee) *)
type point = int * int

(* maximum lst est le plus grand élément de lst
maximum [] vaut 0
pré-condition: les éléments de lst sont positifs *)
let maximum lst =
  let rec max_aux max = function
  | [] -> max
  | t::q -> if t > max then max_aux t q else max_aux max q
in max_aux 0 lst

let test_max = maximum [1; 2; 3; 4; 5; 6; 7; 8; 9; 10]
let _ = if test_max = 10 then print_string "test_max ok" else print_string "test_max pas ok"




(* liste_points abs ords est la liste des points dont la première abscisse est abs et
les ordonnees sont donnees par ords, dans l'ordre decroissant des ordonnées *)
let liste_points abs ords =
  let rec aux abs = function
  | [] -> []
  | t::q -> (abs, t)::aux abs q
in aux abs ords

(* affiche les points en ascii art, avec espace pour les pixel blancs et x pour les pixels noirs *)
let affiche largeur lst =
  let rec aux = function
    | [] -> ()
    | (abs, ord)::q -> print_string (String.make ord ' ' ^ "x\n"); aux q
  in aux (liste_points largeur lst)

  affiche 10 [1; 2; 3; 4; 5; 6; 7; 8; 9; 10]