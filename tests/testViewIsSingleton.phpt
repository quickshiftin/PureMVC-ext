--TEST--
ensure the same instance is always returned by View::getInstance
--FILE--
<?php
$f = View::getInstance();
$f2 = View::getInstance();
if( $f instanceof View &&
	$f2 instanceof View )
	var_dump($f === $f2);
else {
	var_dump($f);
	var_dump($f2);
}
?>
--EXPECT--
bool(true)
