
CREATE TABLE IF NOT EXISTS `menu_ucp` (
  `id` int(10) NOT NULL AUTO_INCREMENT,
  `name` text,
  `link` varchar(255) DEFAULT '#',
  `image` varchar(150) DEFAULT NULL,
  `order` int(11) DEFAULT NULL,
  `direct_link` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'Is it a direct link or not? is needed for the ajax.',
  `permission` varchar(100) DEFAULT NULL,
  `permissionModule` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=114 ;

INSERT INTO `menu_ucp` (`id`, `name`, `link`, `image`, `order`, `direct_link`, `permission`, `permissionModule`) VALUES
(105, '{"english":"Vote panel"}', 'vote', '28.jpg', 105, 0, 'view', 'vote'),
(106, '{"english":"Donate panel"}', 'donate', '20.jpg', 106, 0, 'view', 'donate'),
(108, '{"english":"Item store"}', 'store', '39.jpg', 108, 0, 'view', 'store'),
(109, '{"english":"Account settings"}', 'ucp/settings', '2.jpg', 109, 0, 'canUpdateAccountSettings', 'ucp'),
(110, '{"english":"Change expansion"}', 'ucp/expansion', '21.jpg', 110, 0, 'canChangeExpansion', 'ucp'),
(111, '{"english":"Teleport hub"}', 'teleport', '24.jpg', 111, 0, 'view', 'teleport'),
(112, '{"english":"GM panel"}', 'gm', '31.jpg', 112, 0, 'view', 'gm'),
(113, '{"english":"Admin panel"}', 'admin', '23.jpg', 113, 1, 'view', 'admin');
