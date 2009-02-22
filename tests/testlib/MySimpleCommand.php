<?php
class MySimpleCommand extends SimpleCommand {
	public function execute($notification) {
		echo $notification->getName();
	}
}
