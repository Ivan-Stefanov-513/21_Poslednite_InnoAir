<?php
// Connection checking:
// echo "<h1>test<h2>";

$link = mysqli_connect('localhost', 'poslednite', 'drenki_gl0g', 'poslednite');
if (!$link)
{
    echo "<h2>MySQL Error!</h2>";   
    exit;
}
// $_GET["ID"]
// Switch to a different database:

// table header output:
// echo "<table border=\"1\" width=\"100%\" bgcolor=\"#FFFFE1\">";
// echo "<tr><td>Value1</td><td>Value2</td><td>Value3</td>";
// SQL-request:
$ID=$_GET['ID'];
$TABLE=$_GET['TABLE'];


$db="poslednite";
mysqli_select_db($link, $db);

$q = mysqli_query ($link, "SELECT * FROM `$TABLE` WHERE `station_id` = $ID  LIMIT 2000");
// table-result output
$time ="[";
$nums ="[";


for ($c=0; $c<mysqli_num_rows($q); $c++)
{
    // echo "<tr>";
    $f = mysqli_fetch_array($q); // Returns an array that corresponds to the fetched row and moves the internal data pointer ahead.


    if ($c<mysqli_num_rows($q) -1) {
        $time .=  $f[2]. ",";
        $nums .= '"'. $f[3]  . '"'  . ",";
    }
    else
    {
          $time .= $f[2] ;
          $nums .= '"'. $f[3]. '"'  ;
    }
    // echo "<td>$f[1]</td><td>$f[2]</td><td>$f[3]</td>";
    // echo "</tr>";
}
echo "$time" . "]|";
echo "$nums". "]" ;
?>


