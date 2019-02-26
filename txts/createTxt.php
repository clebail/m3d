<?php
$handle = fopen($_SERVER["argv"][1], "r");

$couleur = isset($_SERVER["argv"][2]) ? $_SERVER["argv"][2] : 0;
$nbCouche = isset($_SERVER["argv"][3]) ? $_SERVER["argv"][3] : 100;
$append = isset($_SERVER["argv"][4]);

if ($handle) {
	$content = "";
	$fileIdx = $nbCouche;

	while (($line = fgets($handle)) !== false) {
        $line = trim($line);
        
        if($line != "") {
			$items = explode(" ", $line);
			if($items[0] == "0") {
				if($content != "") {
					write($fileIdx, $content, $append);
				}
				
				$content = "";
				$fileIdx--;
			} else {			
				$x = ((intval($items[2]) - 390) / 20) * 15;
				$y = ((intval($items[4]) - 390) / 20) * 15;
			
				$content .= "{$x};{$y};{$couleur};1\n";
			}
        }
    }
    
    if($content != 0) {
		write($fileIdx, $content, $append);
    }

	fclose($handle);
}

function write($fileIdx, $content, $append) {
	$fileName = str_pad($fileIdx, 4, "0", STR_PAD_LEFT).".txt";
	
	if($append) {
		$content = file_get_contents($fileName).$content;
	}
	
	file_put_contents($fileName, $content);
}
