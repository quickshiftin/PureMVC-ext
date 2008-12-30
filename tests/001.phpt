--TEST--
determine if pure_mvc extension is loadable
--FILE--
<?php 
if(extension_loaded('pure_mvc'))
	echo 'pure_mvc extension is available';
?>
--EXPECT--
pure_mvc extension is available
