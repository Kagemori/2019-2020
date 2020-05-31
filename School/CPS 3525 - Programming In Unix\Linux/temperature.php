<?php
	if (!empty($_SERVER['HTTP_CLIENT_IP'])) {
		$ip = $_SERVER['HTTP_CLIENT_IP'];
	} elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) {
		$ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
	} else {
		$ip = $_SERVER['REMOTE_ADDR'];
	}
	
	echo "IP: " . $ip . "<br>";
	
	//Call API
	$state = file_get_contents('https://ipapi.co/region/');
	$country = file_get_contents('https://ipapi.co/country_name/');
	$timezone = file_get_contents('https://ipapi.co/timezone/');
	$latlong = explode(",", file_get_contents('https://ipapi.co/' . $ip . '/latlong/'));
	$weather = file_get_contents('http://api.openweathermap.org/data/2.5/weather?lat=' . $latlong[0] . '&lon=' . $latlong[1] . '&appid=3f27fbb27724fb79e747efdff81ade5c');
	$json = json_decode($weather,true);	
	
	//Display all data
	echo "City: " . $json["name"] . "<br>";
	echo "State: " . $state . "<br>";
	echo "Country: " . $country . "<br>";
	echo "Description: " . $json["weather"][0]["description"] . "<br>";
	
	//Calculate temperature, converting Kelvin to C and F
	$temp = $json["main"]["temp"];
	$celsius = ($temp - 273.15);
	$fahrenheit = ($temp * (9/5) - 459.67);
	echo "Temperature: " . $celsius . " C = " . $fahrenheit . " F<br>";
	
	echo "Humidity: " . $json["main"]["humidity"] . "%<br>";
	
	//Convert meters to miles
	$miles = $json["visibility"] / 1609;
	echo "Visibility: " . $json["visibility"] . " m = " . $miles . " miles<br>";
	
	echo "Pressure: " . $json["main"]["pressure"] . " hpa<br>";
	
	//Convert m/s to mph
	$wmph = ($json["wind"]["speed"] * 2.237);
	echo "Wind Speed: " . $json["wind"]["speed"] . " m/s = " . $wmph . " mph<br>";
	
	//Determine wind direction based on degrees
	$deg = $json["wind"]["deg"];
	if($deg >= 22.5 && $deg <= 67.5){
		$dir = "NE";
	}elseif($deg >= 67.5 && $deg <= 112.5){
		$dir = "E";
	}elseif($deg >= 112.5 && $deg <= 157.5){
		$dir = "SE";
	}elseif($deg >= 157.5 && $deg <= 202.5){
		$dir = "S";
	}elseif($deg >= 202.5 && $deg <= 247.5){
		$dir = "SW";
	}elseif($deg >= 247.5 && $deg <= 292.5){
		$dir = "W";
	}elseif($deg >= 292.5 && $deg <= 337.5){
		$dir = "NW";
	}else{
		$dir = "N";
	}
	echo "Wind Direction: " . $dir . " (" . $deg . ")<br>";
	
	echo "Timezone: " . $timezone . "<br>";
	date_default_timezone_set($timezone);
	echo "Sunrise: " . date("Y-m-d H:i:s", $json["sys"]["sunrise"]) . "<br>";
	echo "Sunset: " . date("Y-m-d H:i:s", $json["sys"]["sunset"]) . "<br>";
?>