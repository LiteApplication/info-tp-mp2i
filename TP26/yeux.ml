(*IMAGES.ml*)

type couleur = int array
type image = {
    max : int;
    pix : couleur array array
  }

let noir : couleur = [|0;0;0|]
           
(** lecture image PPM P3 sans commentaires *)
let charger_image fichier =
  let file = open_in fichier in
  let _ = input_line file in
  let dim = Array.of_list (List.map int_of_string (String.split_on_char ' ' (input_line file)) ) in
  let haut = dim.(1) in
  let larg = dim.(0) in
  let mmm = int_of_string (input_line file) in
  let img = { max = mmm; pix = (Array.init haut) (fun i -> Array.init larg (fun j -> [|0;0;0|])) } in
  let i = ref 0 in
  let j = ref 0 in
  let c = ref 0 in
  try
    while true do
      let line = input_line file in
      let not_empty = List.filter (fun x-> String.length x > 0) (String.split_on_char ' ' line) in
      let coeffs = Array.of_list (List.map int_of_string not_empty) in
      for k = 0 to (Array.length coeffs) - 1 do
        img.pix.(!i).(!j).(!c) <- coeffs.(k);
        if !c <> 2 then
          incr c
        else ( c := 0;
               if !j = larg - 1 then
                 ( j := 0; incr i )
               else incr j
             )
      done
    done;
    close_in file;
    img
  with End_of_file -> close_in file; img
  

let sauvegarder_image img fichier =
  let file = open_out fichier in
  let larg = Array.length img.pix in
  let haut = Array.length img.pix.(0) in
  output_string file ("P3\n" ^ (string_of_int haut) ^ " " ^ (string_of_int larg) ^ "\n");
  output_string file (string_of_int img.max ^ "\n");
  for i = 0 to larg-1 do
    for j = 0 to haut-1 do
      for c = 0 to 2 do
        output_string file (string_of_int (img.pix.(i).(j).(c)) ^ " ")
      done
    done;
    output_string file "\n"
  done;
  close_out file


(*YEUX_ROUGES.ml*)
let hsv_of_rgb (r, g, b) =
  let rf = float_of_int r /. 255. in
  let gf = float_of_int g /. 255. in
  let bf = float_of_int b /. 255. in
  let cmax = max (max rf gf) bf in
  let cmin = min (min rf gf) bf in
  let diff = cmax -. cmin in
  let h = if cmax = cmin then 0.
          else if cmax = rf then mod_float (60. *. ((gf -. bf) /. diff) +. 360.) 360.
          else if cmax = gf then mod_float (60. *. ((bf -. rf) /. diff) +. 120.) 360.
          else mod_float (60. *. ((rf -. gf) /. diff) +. 240.) 360. in
  let s = if cmax = 0. then 0. else (diff /. cmax) *. 100. in
  let v = cmax *. 100. in
  (h, s, v)
   
let rouge pixel =
  let h, s, _ = hsv_of_rgb (pixel.(0), pixel.(1), pixel.(2)) in
   30.<s && s<325. && ( h < 40. || h > 320. ) 


(* My code *)

type coord = (int * int)

(* let _ = sauvegarder_image (charger_image "photos/photo1.ppm")  "test.ppm"  *)

let voisins (image: image) (x, y: coord): coord list = 
  if (rouge image.pix.(y).(x)) then [] else
  let (h,w) = Array.length image.pix, Array.length image.pix.(0) in
  let voisins_list = [(x-1,y-1);(x-1,y);(x-1,y+1);(x,y-1);(x,y+1);(x+1,y-1);(x+1,y);(x+1,y+1)] in
  List.filter (fun (x,y) -> x>=0 && x<h && y>=0 && y<w && (rouge image.pix.(y).(x))) voisins_list

let ajoute_sommets (queue:  (coord Queue.t)) (coords: coord list ): unit = 
  List.iter (fun c -> Queue.add c queue) coords 

let composante_connexe (img : image) ((x,y) : coord) : bool array = 
  let (h,w) = Array.length img.pix, Array.length img.pix.(0) in
  let queue = Queue.create () in
  let composante = Array.make (h*w) false in 
  Queue.add (x,y) queue;
  while not (Queue.is_empty queue) do
    let (x,y) = Queue.take queue in
    if not composante.(y * w + x) then
      begin
        composante.(y * w + x) <- true;
        ajoute_sommets queue (voisins img (x,y))
      end
  done;
  composante

let modif_coul (img: image) ((x,y) : coord) : unit = 
  let (r, g, b) = img.pix.(y).(x).(0), img.pix.(y).(x).(1), img.pix.(y).(x).(2) in
  img.pix.(y).(x) <- [|(g+b)/2;g;b|]

let enlever_yeux_rouges (img: image) (composante: bool array) : unit = 
  let (h,w) = Array.length img.pix, Array.length img.pix.(0) in
  for i = 0 to h-1 do
    for j = 0 to w-1 do
      if composante.(i*w + j) then begin 
        modif_coul img (i,j);
        print_int i;
        print_string ", ";
        print_int j;
        print_newline () 
      end
    done
  done


let _ = 
  let img = charger_image "photos/photo1.ppm" in
  let bad_pix = [|(427,672)|] in 
  Array.iter (fun (x,y) -> enlever_yeux_rouges img (composante_connexe img (x,y))) bad_pix;
  sauvegarder_image img "test1.ppm"