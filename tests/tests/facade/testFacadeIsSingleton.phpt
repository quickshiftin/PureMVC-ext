--TEST--
ensure the same instance is always returned by Facade::getInstance
--FILE--
<?php
$f = Facade::getInstance();
$f2 = Facade::getInstance();
if( $f instanceof Facade &&
	$f2 instanceof Facade )
	var_dump($f === $f2);
else {
	var_dump($f);
	var_dump($f2);
}
?>
--EXPECT--
bool(true)
