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
            
            <div id="header">
            	
                <div class="top_container">
                
                	<div class="login_box_top">
                    	<div class="actions_cont">
                        
                        	{if $isOnline}
                            	<div class="account_info">
                                	
                                    <!-- Avatar -->
                                    	<div class="avatar_top">
                                            <img src="{$CI->user->getAvatar()}" width="50" height="50"/>
                                            <span></span>
                                        </div>
                                    <!-- Avatar . End -->
                                    
                                    <!-- Welcome & VP/DP -->
                                	<div class="left">
                                    
                                        <p>Welcome back, <span>{$CI->user->getUsername()}</span>!</p>
                                        <div class="vpdp">
                                        
                                        	<div class="vp">
                                           		<img src="{$url}application/images/icons/lightning.png" align="absmiddle" width="12" height="12" /> VP
                                                <span>{$CI->user->getVp()}</span>
                                            </div>
                                            <div class="dp">
                                           		<img src="{$url}application/images/icons/coins.png" align="absmiddle" width="12" height="12"  /> DP
                                                <span>{$CI->user->getDp()}</span>
                                            </div>
                                            
                                        </div>
                                    </div>
                                    <!-- Welcome & VP/DP . End-->
                                    	<div class="right">
                                        	<a href="{$url}ucp" class="nice_button">User panel</a>
                                            <a href="{$url}vote" class="nice_button">Vote</a>
											<a href="{$url}logout" class="nice_button">Log out</a>
                                        </div>
                                    <!-- Account Panel & Logout -->
                                    
                                </div>
                            {else}
                            	<div class="login_form_top">
                                    {form_open('login')}
                                            <input type="text" name="login_username" id="login_username" value="" placeholder="Username">
                                            <input type="password" name="login_password" id="login_password" value="" placeholder="Password">
                                            <input type="submit" name="login_submit" value="Login">
                                    </form>
                            	</div>
                            {/if}
                            
                        </div>
                    </div>
                
                    <div class="top_menu">
                        <ul id="top_menu">
                            {foreach from=$menu_top item=menu_1}
                                <li><a {$menu_1.link}>{$menu_1.name}</a></li>
                            {/foreach}
                        </ul>
                    </div>
                </div>
                
                <a id="server_logo" href="./" title="{$serverName}"><p>{$serverName}</p></a>
                
				<div id="flash_content">
                	<object id="animation" 
                    width="1026" height="359" 
                    align="middle" 
                    type="application/x-shockwave-flash" 
                    name="animation" 
                    data=" {$url}application/themes/crusader_theme/flash/animation.swf">
                        <param name="quality" value="low">
                        <param name="bgcolor" value="#1b1b1b">
                        <param name="play" value="true">
                        <param name="loop" value="true">
                        <param name="wmode" value="direct">
                        <param name="menu" value="true">
                        <param name="devicefont" value="false">
                        <param name="salign" value="">
                        <param name="wmode" value="opaque" />
                    </object>
                    
                    <!--<a href="http://www.adobe.com/go/getflash">-->
                    <!--<img src="http://www.adobe.com/images/shared/download_buttons/get_flash_player.gif" alt="Get Adobe Flash player" />-->
                    <!--</a>-->
                    <!--<p>This page requires Flash Player version 10.2.153 or higher.</p>-->
                </div>
                
                <!--<div class="accp_register">
                    {if $isOnline}
                        <a href="./ucp" id="accp_button"><h1>Account Panel</h1></a>
                        {else}
                        <a href="./register" id="register_button"><h1>Register</h1></a> 
                    {/if}
                </div>-->
                
                <!--{$serverName}-->
           	</div>
            
            <div class="sword"></div>
            
            <div class="top_border"></div>
			<div id="main">
            
                
				<aside id="left">
                
                	{if $isOnline}
                		<a href="{$url}vote" id="vote_banner"><p>Vote for us</p></a>
                    {else}
                    	<a href="{$url}register" id="register_banner"><p>Create Account</p></a>
                    {/if}
                    
                    <article>
						<ul id="left_menu">
							{foreach from=$menu_side item=menu_2}
								<li><a {$menu_2.link}><img src="{$image_path}bullet.png">{$menu_2.name}</a><p></p></li>
							{/foreach}
                            <li class="bot_shadow"></li>
						</ul>
					</article>                   
                   	
                    {foreach from=$sideboxes item=sidebox}
						<article class="sidebox">
							<h1 class="top"><p>{$sidebox.name}</p></h1>
							<section class="body">
								{$sidebox.data}
							</section>
						</article>
					{/foreach}
                    
				</aside>

				<aside id="right">
					<section id="slider_bg" {if !$show_slider}style="display:none;"{/if}>
						<div id="slider">
                        	<div class="overlay"></div>
							{foreach from=$slider item=image}
								<a href="{$image.link}"><img src="{$image.image}" title="{$image.text}"/></a>
							{/foreach}
						</div>
                        <h1 id="news_title"><p>Latest News</p></h1>
					</section>
					{$page}
				</aside>

				<div class="clear"></div>
			</div>
			<footer>
            	<h3>{$serverName} &copy; Copyright 2012 </h3>
             	<a href="http://evil.duloclan.com" id="evil-logo" target="_blank" title="Design by EvilSystem"><span>Design by EvilSystem</span></a>
				<a href="http://raxezdev.com/fusioncms" id="fcms-logo" target="_blank"><span>Powered by Fusion CMS V6</span></a>
			</footer>
		</section>
	</body>
</html>