<?php

$img = imagecreatetruecolor(1024, 78*15);

$cols[0] = imagecolorallocate($img, 0, 0, 0);
$cols[1] = imagecolorallocate($img, 0, 0, 255);
$cols[4] = imagecolorallocate($img, 255, 0, 0);
$cols[14] = imagecolorallocate($img, 255, 255, 0);
$cols[15] = imagecolorallocate($img, 255, 255, 255);
$cols[70] = imagecolorallocate($img, 0x58, 0x2A, 0x12);
$cols[78] = imagecolorallocate($img, 0xF6, 0xD7, 0xB3);

imagefill($img, 1, 1, $cols[15]);

for($i=1;$i<=78;$i++) {
	$fileName = str_pad($i, 4, "0", STR_PAD_LEFT).".txt";
	
	$handle = fopen($fileName, "r");
	if ($handle) {
		$maxx = array();
		$datas = array();
		while (($line = fgets($handle)) !== false) {
			$items = explode(";", $line);
			$x = $items[0];
			$y = $items[1];
			$couleur = $items[2];
			
			if(!array_key_exists($y, $maxx) || $maxx[$y] < $x) {
				$maxx[$y] = $x;
				$datas[$y] = $couleur;
			}
		}
		
		foreach($datas as $x => $couleur) {
			imagerectangle($img, $x+512, ($i-1)*15, $x+512+15, $i*15, $cols[$couleur]);
			imagefill($img, $x+513, ($i-1)*15 +1, $cols[$couleur]);
		}

		fclose($handle);
	} 
}

imagepng($img, "test.png");
imagedestroy($img);
