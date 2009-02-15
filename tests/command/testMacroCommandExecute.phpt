--TEST--
MacroCommand::execute(), ensure calls to each commands execute methods are invokded
--SKIPIF--
<?php if (!extension_loaded("pure_mvc")) print "skip"; ?>
--FILE--
<?php 
class SubCommand {
	static $id = 0;

	public function __construct() {
		self::$id++;
	}
	public function execute() {
		echo self::$id . PHP_EOL;
	}
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
?>
--EXPECT--
1
2
