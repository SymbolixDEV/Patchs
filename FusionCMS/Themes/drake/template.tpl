{$head}
	<body>
<script type="text/javascript" src="https://d31qbv1cthcecs.cloudfront.net/atrk.js"></script><script type="text/javascript">_atrk_opts = { atrk_acct: "vHn9g1a8n500MP", domain:"drakewow.com"}; atrk ();</script><noscript><img src="https://d5nxst8fruw4z.cloudfront.net/atrk.gif?account=vHn9g1a8n500MP" style="display:none" height="1" width="1" alt="" /></noscript>	
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
			<ul id="top_menu">
				{foreach from=$menu_top item=menu_1}
					<li><a {$menu_1.link}>{$menu_1.name}</a></li>
				{/foreach}
			</ul>
			<div id="main">
				<aside id="left">
                
                	<!-- REGISTER Banner -->
                    	<a href="register" class="register"><p></p><span></span></a>
                    <!-- REGISTER Banner . End -->
					
					{foreach from=$sideboxes item=sidebox}
						<article>
							<h1 class="top">{$sidebox.name}</h1>
							<section class="body">
								{$sidebox.data}
							</section>
						</article>
					{/foreach}
                    
                    <!-- VOTE Banner -->
                    	<a href="vote" class="vote-b"><p></p><span></span></a>
                    <!-- VOTE Banner . End -->
                    
                    
						<ul id="left_menu">
							{foreach from=$menu_side item=menu_2}
								<li><a {$menu_2.link}><img src="{$image_path}bullet.png">{$menu_2.name}</a></li>
							{/foreach}
						</ul>
					
				</aside>

				<aside id="right">
					<section id="slider_bg" {if !$show_slider}style="display:none;"{/if}>
						<div id="slider">
							{foreach from=$slider item=image}
								<a href="{$image.link}"><img src="{$image.image}" title="{$image.text}"/></a>
							{/foreach}
						</div>
					</section>

					{$page}
				</aside>

				<div class="clear"></div>
			</div>
			<footer>
                <center>
                    <div id="logos">
                    	<a href="http://evil.duloclan.com" rel="nofollow" id="evillogo" target="_blank"></a>
                        <a href="http://raxezdev.com/fusioncms" rel="nofollow" id="cmslogo" target="_blank"></a>
                    </div>
                    <div id="siteinfo">
                        All righs reserved © <font color="#695946">DrakeWoW</font> <br/>
                        This website and its content was created for <a href="/">DrakeWoW.com</a> <br/>
						Partners: <a href="http://topg.org/World-Of-Warcraft"> Wow Vanilla Servers </a>
                    </div>
                </center>
			</footer>
		</section>
	</body>
</html>