CREATE TABLE IF NOT EXISTS `cta_logs` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `account` int(20) NOT NULL DEFAULT '0',
  `account_name` varchar(150) NOT NULL,
  `character` bigint(20) NOT NULL DEFAULT '0',
  `character_name` varchar(150) NOT NULL,
  `dest_account` bigint(20) NOT NULL DEFAULT '0',
  `dest_account_name` varchar(150) NOT NULL,
  `realm_id` tinyint(3) NOT NULL DEFAULT '0',
  `price` int(10) NOT NULL DEFAULT '0',
  `currency` varchar(10) NOT NULL,
  `start_currency` int(10) NOT NULL DEFAULT '0',
  `end_currency` int(10) NOT NULL DEFAULT '0',
  `time` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1;