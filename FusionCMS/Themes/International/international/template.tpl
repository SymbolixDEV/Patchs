{$head}
	<body>
		<!--[if lte IE 8]>
			<style type="text/css">
				body {
					background-image:url(images/bg.jpg);
					background-position:top center;
				}
			</style>
		<![endif]-->
		<section id="wrapper">
			{$modals}
			<header id="hand"></header>
			<header><a href="http://facebook.com" id="hand2" target="_blank"></a></header>
			<header><a href="http://youtube.com" id="hand3" target="_blank"></a></header>
			<header><a href="http://twitter.com" id="hand4" target="_blank"></a></header>
			<header id="hand5"></header>
			<header><a href="register" id="hand6"></a></header>
			<header id="hand7"></header>
			<header id="marquee1"><marquee scrollAmount="3" scrollDelay="3" direction="up" width="1200px" height="310px" behavior="scroll" loop="500">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">
<img src="http://www.evo-network.eu/Fusion/application/themes/international/images/marquee1.png">

</marquee></header>

			<ul id="top_menu">
				{foreach from=$menu_top item=menu_1}
					<li><a {$menu_1.link}>{$menu_1.name}</a></li>
				{/foreach}
			</ul>
			<div id="main">
				<aside id="left">
						<article>
				<h1 class="top"></h1>
				<div id="yt"><center><iframe width="278" height="184" src="http://www.youtube.com/embed/-Gqo1lD1skQ?wmode=transparent" frameborder="0" wmode="Opaque"></iframe></center></div>
						</article>
					<article>
						<h1 class="top">Navigation</h1>
						<ul id="left_menu">
							{foreach from=$menu_side item=menu_2}
								<li><a {$menu_2.link}><img src="{$image_path}bullet.png">{$menu_2.name}</a></li>
							{/foreach}
						</ul>
					</article>

					{foreach from=$sideboxes item=sidebox}
						<article>
							<h1 class="top">{$sidebox.name}</h1>
							<section class="body">
								{$sidebox.data}
							</section>
						</article>
					{/foreach}
				</aside>

				<aside id="right">
					<section id="slider_bg" {if !$show_slider}style="display:none;"{/if}>
						<div id="slider">
							<div id="slider_frame">
							{foreach from=$slider item=image}
								<a href="{$image.link}"><img src="{$image.image}" title="{$image.text}"/></a>
							{/foreach}
							</div>
						</div>
					</section>

					{$page}
				</aside>

				<div class="clear"></div>
			</div>
			<footer>
				<a href="http://raxezdev.com/fusioncms" id="logo" target="_blank"></a><a href="http://www.facebook.com/LoomNetwork" id="logo2" target="_blank"></a>
				<p>&copy; Copyright 2012 {$serverName}</p>
			</footer>
		</section>
	</body>
<link href='http://fonts.googleapis.com/css?family=Federant' rel='stylesheet' type='text/css'>
</html>