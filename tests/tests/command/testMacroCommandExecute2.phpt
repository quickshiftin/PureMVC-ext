--TEST--
MacroCommand::execute(), ensure all commands are popped off the internal queue after execute() has completed
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
include(dirname(__FILE__) . '/../../testlib/include.php');
MyMacroCommand::setNumCommands(2);
$macroCmd = new MyMacroCommand();
$macroCmd->execute(new MyNotification());
var_dump($macroCmd);
?>
--EXPECT--
object(MyMacroCommand)#1 (2) {
  ["facade:protected"]=>
  string(0) ""
  ["subCommands:private"]=>
  array(0) {
  }
}
