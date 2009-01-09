--TEST--
MacroCommand::addSubCommand(), ensure addSubCommand actually stores the ICommand in an internal array
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
class SubCommand {}
class MyMacroCommand extends MacroCommand {
	protected function initializeMacroCommand() {
		echo 'holy crap' . PHP_EOL;
		$this->addSubCommand(new SubCommand());
	}
	public function blah() {
		$this->initializeMacroCommand();
	}
}
$macroCmd = new MyMacroCommand();
//$macroCmd->blah();
var_dump($macroCmd);
?>
--EXPECT--
object(MacroCommand)#1 (2) {
  ["facade:protected"]=>
  string(0) ""
  ["subCommands:private"]=>
  array(1) {
    [0]=>
    object(SubCommand)#2 (0) {
    }
}
