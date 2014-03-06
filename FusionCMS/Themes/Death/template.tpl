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
			<ul>
				<a id="logo_anch" href="#" title="">The Death<!-- or any title--></a>
			</ul>
			<header id="hand"></header>
			<div id="menu_bg">
	            <ul id="top_menu">
					{foreach from=$menu_top item=menu_1}
						<li><a {$menu_1.link}>{$menu_1.name}</a></li>
						<li class="menu-border"></li>					
					{/foreach}
				</ul>
			</div>
			<div id="main">
				<aside id="left">
					<article>
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
						<div id="slider_top_border"></div>
                        <div id="slider">
							{foreach from=$slider item=image}
								<a href="{$image.link}"><img src="{$image.image}" title="{$image.text}"/></a>
							{/foreach}
						</div>
					</section>
					<div id="separator_main"></div>
					{$page}
				</aside>

				<div class="clear"></div>
			</div>
			<footer>
				<a href="http://raxezdev.com/fusioncms" id="logo" target="_blank"></a>
				<p>&copy; Copyright 2013 {$serverName}</p>
			</footer>
		</section>
	</body>
</html>