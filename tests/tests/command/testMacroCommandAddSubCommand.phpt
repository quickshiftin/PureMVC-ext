--TEST--
MacroCommand::addSubCommand(), ensure addSubCommand actually stores the ICommand in an internal array
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
include(dirname(__FILE__) . '/../../testlib/include.php');
$macroCmd = new MyMacroCommand();
var_dump($macroCmd);
?>
--EXPECT--
object(MyMacroCommand)#1 (2) {
  ["facade:protected"]=>
  string(0) ""
  ["subCommands:private"]=>
  array(1) {
    [0]=>
    string(10) "SubCommand"
  }
}
