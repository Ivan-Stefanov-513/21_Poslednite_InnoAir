<?php
// Connection checking:
$link = mysqli_connect('212.5.155.204', 'poslednite', 'drenki_gl0g', 'poslednite');
if (!$link)
{
    echo "<h2>MySQL Error!</h2>";   
    exit;
}
// $_GET["ID"]
// Switch to a different database:
$db="poslednite";
mysqli_select_db($link, $db);
// table header output:
echo "<table border=\"1\" width=\"100%\" bgcolor=\"#FFFFE1\">";
echo "<tr><td>Value1</td><td>Value2</td><td>Value3</td>";
// SQL-request:
$q = mysqli_query ($link, "SELECT * FROM `temperature` WHERE `station_id` =  $_GET['ID']");
// table-result output
for ($c=0; $c<mysqli_num_rows($q); $c++)
{
    echo "<tr>";
    $f = mysqli_fetch_array($q); // Returns an array that corresponds to the fetched row and moves the internal data pointer ahead.
    echo "<td>$f[0]</td><td>$f[1]</td><td>$f[5]</td>";
    echo "</tr>";
}
echo "</table>";
?>