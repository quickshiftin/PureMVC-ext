<?php
class MySimpleCommand extends SimpleCommand {
	public function execute(INotification $notification) {
		echo $notification->getName();
	}
}
