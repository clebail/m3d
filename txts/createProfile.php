<?php

$img = imagecreatetruecolor(1024, 78*15);

$blanc = imagecolorallocate($img, 255, 255, 255);
$bleu = imagecolorallocate($img, 0, 0, 255);

imagecolortransparent($img, $blanc);

imagefill($img, 1, 1, $blanc);

for($i=1;$i<=intval($_SERVER["argv"][2]);$i++) {
	$fileName = $_SERVER["argv"][1]."/".str_pad($i, 4, "0", STR_PAD_LEFT).".txt";

	echo "Traite {$fileName}\n";
	
	$handle = fopen($fileName, "r");
	if ($handle) {
		$maxx = array();
		while (($line = fgets($handle)) !== false) {
			$items = explode(";", $line);
			$x = $items[0];
			$y = $items[1];
			
			if(!array_key_exists($y, $maxx) || $maxx[$y] < $x) {
				$maxx[$y] = $x;
			}
		}
		
		foreach($maxx as $x => $y) {
			imagerectangle($img, $x+512, ($i-1)*15, $x+512+15, $i*15, $bleu);
			//imagefill($img, $x+513, ($i-1)*15 +1, $fond);
		}

		fclose($handle);
	} 
}

imagepng($img, $_SERVER["argv"][1].".png");
imagedestroy($img);
