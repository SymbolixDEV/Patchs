CREATE TABLE `recruit_a_friend_claimed` (
	`recruiter` INT(10) NOT NULL DEFAULT '0',
	`referral` INT(10) NOT NULL DEFAULT '0',
	PRIMARY KEY (`recruiter`, `referral`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB;

ALTER TABLE `pending_accounts`
	ADD COLUMN `recruiter` INT(10) NULL AFTER `key`;