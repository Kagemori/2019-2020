<?php
//Variables for information on the file to be uploaded
$target_dir = "upload/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$tmpfile = basename($_FILES["fileToUpload"]["tmp_name"]);
$fileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
$uploadedFile = basename($_FILES["fileToUpload"]["name"]);
$filesize = $_FILES["fileToUpload"]["size"];
$uploadOk = 1;

//Checks if a file to upload was already uploaded to the server, this deletes the old file
if(file_exists($target_file)){
	unlink($target_file);	
}

//Systematically checks the upload file's size, type and then amount of columns
//Check if file is between 10 and 1000 Bytes
if($filesize < 10 || $filesize > 1000) {
    echo "File size is not within the range, upload failed! <br>";
	$uploadOk = 0;
} else {
	//Check if file type is csv
	if($fileType != "csv"){
		echo "File is in wrong format. Only text format is allowed. <br>";
		$uploadOk = 0;
	} else {
		//Creates an array to store the first line in the csv file
		$array = fopen($_FILES["fileToUpload"]["tmp_name"],"r");
		
		//Counts how many elements were in the first line, each element represents a column, there must be 4 columns to upload
		if(count(fgetcsv($array)) == 4){
			//File is suitable for upload as it passes all 3 requirements
		}else{
			echo "The text has wrong number of columns, upload failed! <br>";
			$uploadOk = 0;
		}
		fclose($array);
	}	
}

//Check if it was uploaded successfully
$new_file = "upload/" . $_FILES["fileToUpload"]["name"];
if($uploadOk == 0){
	exit();
}else{
	if(move_uploaded_file($_FILES["fileToUpload"]["tmp_name"],$new_file)){
		//Present file information at the top to the user
		echo "Temp File: /tmp/$tmpfile <br> File Path: $target_file <br>";
		echo "File Size: ". $_FILES["fileToUpload"]["size"] . " bytes <br>";
		echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded. <br><br>";
	}else{
		echo "Upload failed! <br><br>";
		exit();
	}
}


//Convert to a multidimensional array
$temparray = fopen($target_file,"r");
for($i = 0; $i <= count(file($target_file)); $i++){
	$csv[] = fgetcsv($temparray);
}
//Removes the first line that contains the header
// Removes the last line that contains an empty array
unset($csv[0]); unset($csv[count($csv)]);

//Function to make a table of the roster, has 2 inputs: 
// the multidimensional array containing the grades and students, 
// the subject filter dictated by user input,
function makeTable($array, $subject){
	//Create the starting header for the table
	if($subject == "Math"){
		$start = 1;
		$end = 1;
		echo "<table border='4' class='stats' cellspacing='0'>
			<tr>
            <th>#</th>
            <th>Name</th>
            <th>Math</th>
            <th>Avg</th>
            </tr>";
	}elseif($subject == "Science"){
		$start = 2;
		$end = 2;
		echo "<table border='4' class='stats' cellspacing='0'>
			<tr>
            <th>#</th>
            <th>Name</th>
			<th>Science</th>
            <th>Avg</th>
            </tr>";
	}elseif($subject == "English"){
		$start = 3;
		$end = 3;
		echo "<table border='4' class='stats' cellspacing='0'>
			<tr>
            <th>#</th>
            <th>Name</th>
			<th>English</th>
            <th>Avg</th>
            </tr>";
	}elseif($subject == "All"){
		$start = 1;
		$end = 3;
		echo "<table border='4' class='stats' cellspacing='0'>
			<tr>
            <th>#</th>
            <th>Name</th>
            <th>Math</th>
			<th>Science</th>
			<th>English</th>
            <th>Avg</th>
            </tr>";
	}
	
	//Initialized variables that are going to be used, was not sure if php required initialization for scope
	$stdavg = 0; $c1avg = 0; $c2avg = 0; $c3avg = 0; $g1 = 0; $g2 = 0; $g3 = 0;
	
	//Reads multidimentional array, inputing each grade as well as summing the student's individual average 
	// and the overall class average of a particular subject
	for($i = 1; $i <= (count($array)); $i++){
		for($j = $start; $j <= $end; $j++){
			if($j == 1){
				$g1 = $array[$i][$j];
				$stdavg += $g1;
				$c1avg += $g1;
			}elseif($j == 2){
				$g2 = $array[$i][$j];
				$stdavg += $g2;
				$c2avg += $g2;
			}elseif($j == 3){
				$g3 = $array[$i][$j];
				$stdavg += $g3;
				$c3avg += $g3;
			}
		}
		$stdname = $array[$i][0];
		$stdavg = round($stdavg / (($end - $start) + 1), 1);
		
		//Prints to table each student's grades and average based on filter
		if($subject == "Math"){
			echo "<tr>
				<td> $i </td>
				<td> $stdname </td>
				<td> $g1 </td>
				<td> $stdavg </td></tr>";
		}elseif($subject == "Science"){
			echo "<tr>
				<td> $i </td>
				<td> $stdname </td>
				<td> $g2 </td>
				<td> $stdavg </td></tr>";
		}elseif($subject == "English"){
			echo "<tr>
				<td> $i </td>
				<td> $stdname </td>
				<td> $g3 </td>
				<td> $stdavg </td></tr>";
		}elseif($subject == "All"){
			echo "<tr>
				<td> $i </td>
				<td> $stdname </td>
				<td> $g1 </td>
				<td> $g2 </td>
				<td> $g3 </td>
				<td> $stdavg </td></tr>";
		}
		$stdavg = 0;
	}
	
	//Calculates total class averages and prints them to the end of the table based on user input
	$c1avg = round($c1avg / (count($array)),1);
	$c2avg = round($c2avg / (count($array)),1);
	$c3avg = round($c3avg / (count($array)),1);
	if($subject == "Math"){
		$totalavg = $c1avg;
		echo "<tr>
				<td>  </td>
				<td> Avg </td>
				<td> $c1avg </td>
				<td> $totalavg </td></tr></table>";
	}elseif($subject == "Science"){
		$totalavg = $c2avg;
		echo "<tr>
				<td>  </td>
				<td> Avg </td>
				<td> $c2avg </td>
				<td> $totalavg </td></tr></table>";
	}elseif($subject == "English"){
		$totalavg = $c3avg;
		echo "<tr>
				<td>  </td>
				<td> Avg </td>
				<td> $c3avg </td>
				<td> $totalavg </td></tr></table>";
	}elseif($subject == "All"){
		$totalavg = round(($c1avg + $c2avg + $c3avg)/3,1);
		echo "<tr>
				<td>  </td>
				<td> Avg </td>
				<td> $c1avg </td>
				<td> $c2avg </td>
				<td> $c3avg </td>
				<td> $totalavg </td></tr></table>";
	}
}

//Sorting function that is based on the average per student
// It finds the averages of each student and sorts/creates a new array based on it
function sortAndAvg(&$csv,$subject){
	for($i = 1; $i <= (count($csv)); $i++){
		if($subject == "Math"){
			$csv[$i][4] = $csv[$i][1];
		}elseif($subject == "Science"){
			$csv[$i][4] = $csv[$i][2];
		}elseif($subject == "English"){
			$csv[$i][4] = $csv[$i][3];
		}elseif($subject == "All"){
			$csv[$i][4] = round((($csv[$i][1] + $csv[$i][2] + $csv[$i][3]) / 3),1);
		}
	}
	usort($csv,avgCmp);
	//Because sorting "reset" the start of the array to 0 the following shifts it back to starting at 1
	// this is because removing the first part of the array initally affected the array elements
	array_unshift($csv,"shift");
	unset($csv[0]);
}

//Custom comparison function for sorting
function avgCmp($a,$b){
	if ( $a[4] == $b[4] )
        return 0;
    if ( $a[4] > $b[4] )
         return -1;
    return 1;
}

//Prints original data in original format
echo "Original Data";
makeTable($csv, "All");

//Print roster in sorted format
echo "<br><br> After sorting by subject: " . $_POST["subject"] . "<br>";
sortAndAvg($csv,$_POST["subject"]);
makeTable($csv,$_POST["subject"]);
?>