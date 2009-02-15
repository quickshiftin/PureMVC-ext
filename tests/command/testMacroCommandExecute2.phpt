--TEST--
MacroCommand::execute(), ensure all commands are popped off the internal queue after execute() has completed
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
class SubCommand {
	public function execute() {}
}
class MyNotification implements INotification {
	public function getName() {}
	public function getType() {}
	public function getBody() {}
	public function setName($name) {}
	public function setBody($body) {}
	public function setType($type) {}
	public function toString() {}
}
class MyMacroCommand extends MacroCommand {
	protected function initializeMacroCommand() {
		$this->addSubCommand('SubCommand');
		$this->addSubCommand('SubCommand');
	}
}
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
