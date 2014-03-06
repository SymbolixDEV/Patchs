<!-- Top part -->
<section id="armory_top">
	<section id="armory_bars">
		{if $stats.maxhealth && $stats.maxhealth != "Unknown"}
			<div id="armory_health">Health: <b>{$stats.maxhealth}</b></div>
		{/if}

		
		{if $secondBarValue && $secondBarValue != "Unknown"}
			<div id="armory_{$secondBar}">{ucfirst($secondBar)}: <b>{$secondBarValue}</b></div>
		{/if}
	</section>

	<img class="avatar" src="{$url}application/images/avatars/{$avatar}.gif"/>
	
	<section id="armory_name">
		<h1>{$name} <a class="color-tooltip-{$faction}" href="{$url}guild/{$realmId}/{$guild}">{$guildName}</a></h1>
		<h2 class="color-c{$class}"><b>{$level}</b> {$raceName} {$className}, <i>{$realmName}</i></h2>
	</section>

	<div class="clear"></div>
</section>

<div class="ucp_divider"></div>

<!-- Main part -->
<section id="armory" style="background-image:url({$url}application/images/armory/{$bg}.png)">
	<section id="armory_left">
		<div class="item"><a></a>{$items.head}</div>
		<div class="item"><a></a>{$items.neck}</div>
		<div class="item"><a></a>{$items.shoulders}</div>
		<div class="item"><a></a>{$items.back}</div>
		<div class="item"><a></a>{$items.chest}</div>
		<div class="item"><a></a>{$items.body}</div>
		<div class="item"><a></a>{$items.tabard}</div>
		<div class="item"><a></a>{$items.wrists}</div>
	</section>

	<!--[if LT IE 8]>
		<script type="text/javascript">
			function noIE()
			{
				if(typeof UI != "undefined")
				{
					UI.alert("The armory is not fully compatible with Internet Explorer 8 or below!");
				}
				else
				{
					setTimeout(noIE, 100);
				}
			}

			$(document).ready(function()
			{
				noIE();
			});
		</script>
	<![endif]-->

	<section id="armory_stats">
		<center id="armory_stats_top">
			{if $has_stats}
				<a href="javascript:void(0)" onClick="Character.tab('stats', this)" class="armory_current_tab">
					{lang("attributes", "character")}
				</a>
			{/if}

			{if $pvp.kills !== false || $pvp.honor !== false || $pvp.arena !== false}
				<a href="javascript:void(0)" onClick="Character.tab('pvp', this)" {if !$has_stats}class="armory_current_tab"{/if}>
					{lang("pvp", "character")}
				</a>
			{/if}
		</center>
		
		{if $has_stats}
		<section id="tab_stats" style="display:block;">
			<div style="width:1200px;height:194px;" id="attributes_wrapper">
				<div id="tab_armory_1" style="float:left;">
					<table width="367px" cellspacing="0" cellpadding="0">
						<tr>
							<td width="50%">{lang("str", "character")}</td>
							<td>{if strlen($stats.strength)}{$stats.strength}{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("sta", "character")}</td>
							<td>{if strlen($stats.stamina)}{$stats.stamina}{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("int", "character")}</td>
							<td>{if strlen($stats.intellect)}{$stats.intellect}{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						{if $stats && array_key_exists("spellPower", $stats)}
							<tr>
								<td width="50%">{lang("sp", "character")}</td>
								<td>{if strlen($stats.spellPower)}{$stats.spellPower}{else}{lang("unknown", "character")}{/if}</td>
							</tr>
						{/if}
						
						{if $stats && array_key_exists("attackPower", $stats)}
							<tr>
								<td width="50%">{lang("ap", "character")}</td>
								<td>{if strlen($stats.attackPower)}{$stats.attackPower}{else}{lang("unknown", "character")}{/if}</td>
							</tr>
						{/if}
					</table>

					<center id="armory_stats_next"><a href="javascript:void(0)" onClick="Character.attributes(2)">{lang("next", "character")} &rarr;</a></center>
				</div>

				<div id="tab_armory_2" style="float:left;">
					<table width="367px" cellspacing="0" cellpadding="0">
						{if $stats && array_key_exists("resilience", $stats)}
							<tr>
								<td width="50%">{lang("res", "character")}</td>
								<td>{if strlen($stats.resilience)}{$stats.resilience}{else}{lang("unknown", "character")}{/if}</td>
							</tr>
						{/if}

						<tr>
							<td width="50%">{lang("armor", "character")}</td>
							<td>{if strlen($stats.armor)}{$stats.armor}{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("block", "character")}</td>
							<td>{if strlen($stats.blockPct)}{$stats.blockPct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("dodge", "character")}</td>
							<td>{if strlen($stats.dodgePct)}{$stats.dodgePct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("parry", "character")}</td>
							<td>{if strlen($stats.parryPct)}{$stats.parryPct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
					</table>

					<center id="armory_stats_next">
						<a href="javascript:void(0)" onClick="Character.attributes(1)">&larr; {lang("previous", "character")}</a>
						<a href="javascript:void(0)" onClick="Character.attributes(3)">{lang("next", "character")} &rarr;</a>
					</center>
				</div>

				<div id="tab_armory_3" style="float:left;">
					<table width="367px" cellspacing="0" cellpadding="0">
						<tr>
							<td width="50%">{lang("crit", "character")}</td>
							<td>{if strlen($stats.critPct)}{$stats.critPct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("ranged_crit", "character")}</td>
							<td>{if strlen($stats.rangedCritPct)}{$stats.rangedCritPct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("spell_crit", "character")}</td>
							<td>{if strlen($stats.spellCritPct)}{$stats.spellCritPct}%{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">{lang("spirit", "character")}</td>
							<td>{if strlen($stats.spirit)}{$stats.spirit}{else}{lang("unknown", "character")}{/if}</td>
						</tr>
						<tr>
							<td width="50%">&nbsp;</td>
							<td></td>
						</tr>
					</table>

					<center id="armory_stats_next"><a href="javascript:void(0)" onClick="Character.attributes(2)">&larr; {lang("previous", "character")}</a></center>
				</div>
			</div>
		</section>
		{/if}

		<section id="tab_pvp" {if !$has_stats}style="display:block;"{/if}>
			<table width="367px" cellspacing="0" cellpadding="0">
				{if $pvp.kills !== false}
				<tr>
					<td width="50%">{lang("kills", "character")}</td>
					<td>{if strlen($pvp.kills)}{$pvp.kills}{else}{lang("unknown", "character")}{/if}</td>
				</tr>
				{/if}

				{if $pvp.honor !== false}
				<tr>
					<td width="50%">{lang("honor", "character")}</td>
					<td>{if strlen($pvp.honor)}{$pvp.honor}{else}{lang("unknown", "character")}{/if}</td>
				</tr>
				{/if}

				{if $pvp.arena !== false}
				<tr>
					<td width="50%">{lang("arena", "character")}</td>
					<td>{if strlen($pvp.arena)}{$pvp.arena}{else}{lang("unknown", "character")}{/if}</td>
				</tr>
				{/if}
			</table>
		</section>
	</section>

	<section id="armory_right">
		<div class="item"><a></a>{$items.hands}</div>
		<div class="item"><a></a>{$items.waist}</div>
		<div class="item"><a></a>{$items.legs}</div>
		<div class="item"><a></a>{$items.feet}</div>
		<div class="item"><a></a>{$items.finger1}</div>
		<div class="item"><a></a>{$items.finger2}</div>
		<div class="item"><a></a>{$items.trinket1}</div>
		<div class="item"><a></a>{$items.trinket2}</div>
	</section>

	<section id="armory_bottom">
		<div class="item"><a></a>{$items.mainhand}</div>
		<div class="item"><a></a>{$items.offhand}</div>
		<div class="item"><a></a>{$items.ranged}</div>
	</section>
</section>

<br />
<br />
<section id="armory_mid_info">
	
    <div class="recent-activity">
    	<h3>Recent Achievements</h3>
        
        <ul class="achievements">
        {if $recent_achievements}
        
        	{foreach from=$recent_achievements key=key item=achiev}
                <li class="achievement">
                	<div id="icon">
                        <span class="icon">
                            <span class="icon-frame frame-12">
                                <img src="http://wow.zamimg.com/images/wow/icons/small/{strtolower($achiev.iconname)}.jpg" alt="" width="16" height="16">
                            </span>
                        </span>
                    </div>
                    <div id="info">
                    	<span id="descr">Earned the achievement <a href="http://wotlk.openwow.com/achievement={$achiev.id}" target="_blank" data-tip='<div class="wiki-tooltip"><span class="icon-frame frame-56" style="background-image: url(http://wow.zamimg.com/images/wow/icons/large/{strtolower($achiev.iconname)}.jpg);"></span><h3><span class="q0">{if $achiev.points > 0}{$achiev.points} points{/if}</span>{str_replace("'", "&prime;", $achiev.name)}</h3><span class="color-tooltip-yellow">{str_replace("'", "&prime;", $achiev.description)}</span></div>'>{$achiev.name}</a>{if $achiev.points > 0} for {$achiev.points} points{/if}.</span> <br/><span id="date">{$achiev.date}</span>
                    </div>
                    <div class="clear"></div>
                </li>
            {/foreach}

        {else}
        	<li id="no-records">No records ware found.</li>
        {/if}
        </ul>
    </div>
    
	<div class="professions">
    	<h3>Main Professions</h3>
		<ul>
        	{foreach from=$main_professions key=key item=prof}
            	{if $prof}
                    <li class="profession">
                    	<div class="profile-progress border-3 {if $prof.percent == 100}completed{/if}">
							<div class="bar border-3 hover" style="width: {$prof.percent}%"></div>
							<div class="bar-contents">
								<div class="profession-details">
									<span class="icon">
										<span class="icon-frame frame-12">
											<img src="http://wow.zamimg.com/images/wow/icons/small/{strtolower($prof.icon)}.jpg" alt="" width="16" height="16" />
										</span>
									</span>
									<span class="name">{$prof.name}</span>
									<span class="value">{$prof.value}/{$prof.max}</span>
								</div>
							</div>
						</div>
                    </li>
                {else}
                	<li class="profession profession-empty">No Profession</li>
                {/if}
            {/foreach}
        </ul>
        {if $secondary_professions}
            <h3>Secondary Professions</h3>
            <ul>
                {foreach from=$secondary_professions key=key item=prof}
                    <li class="profession">
                        <div class="profile-progress border-3 {if $prof.percent == 100}completed{/if}">
                            <div class="bar border-3 hover" style="width: {$prof.percent}%"></div>
                            <div class="bar-contents">
                                <div class="profession-details">
                                    <span class="icon">
                                        <span class="icon-frame frame-12">
                                            <img src="http://wow.zamimg.com/images/wow/icons/small/{strtolower($prof.icon)}.jpg" alt="" width="16" height="16" />
                                        </span>
                                    </span>
                                    <span class="name">{$prof.name}</span>
                                    <span class="value">{$prof.value}/{$prof.max}</span>
                                </div>
                            </div>
                        </div>
                    </li>
                {/foreach}
            </ul>
        {/if}
    </div>
    
	<div class="clear"></div>
</section>

<br />
<br />
<section id="armory_talents" class="talents-expansion-{$expansion_str}">

    <div class="talents-specs">
        {if $talent_specs}
            {foreach from=$talent_specs key=key item=spec}
            <a href="javascript: void(0);" class="talents-spec {if $spec.active}talents-spec-active{/if}" specId="{$key}">
                <div>
                    <div class="talents-spec-icon">
                        {if $spec.icon}
                            <img src="http://wow.zamimg.com/images/wow/icons/medium/{$spec.icon}.jpg" style="vertical-align:middle"> 
                        {else}
                            <img src="http://wow.zamimg.com/images/wow/icons/medium/inv_misc_questionmark.jpg" style="vertical-align:middle"> <br />
                        {/if}
                    </div>
                    <div class="talents-spec-info">
                        <span id="title">{$spec.title}</span><br />
                        <span id="points">{$spec.points}</span>
                        <div class="clear"></div>
                    </div>
                    {if $spec.active}
                        <div class="talents-spec-char-selected"></div>
                 	{/if}
                    <div class="clear"></div>
                </div>
            </a>
            {/foreach}
            <div class="clear"></div>
        {/if}
    </div>
	
    {if $talent_tables}
        {foreach from=$talent_tables key=key item=table}
            <div class="talents" specId="{$key}" style="{if $key != $talent_active_spec}display: none;{/if}"> 
               	
               	<h3>Talents</h3>
                <div class="talents-body">
                    
                    {foreach from=$table key=tab_key item=tab}
                        <div class="talents-tree" id="tree-{$tab.order + 1}" style="float: left;">
                            
                            <div class="talents-tree-bg {$className_clean}-{$tab.order + 1}"></div>
                            
                            <div class="talents-tree-title">
                                <img src="http://wow.zamimg.com/images/wow/icons/small/{$tab.icon}.jpg" height="21px" style="vertical-align:middle"> 
                                {$tab.name}
                            </div>
                            
                            <table cellpadding="0" cellspacing="0" border="0">
                                {foreach from=$tab.table key=row_key item=row}
                                    <tr>
                                        {foreach from=$row key=col_key item=talent}
                                            <td>
                                                {if $talent}
                                                    <div class="iconmedium">
                                                        <ins style="background-image: url(http://wow.zamimg.com/images/wow/icons/medium/{strtolower($talent.icon)}.jpg);"></ins>
                                                        <del></del>
                                                        <a href="javascript: void(0);" data-tip-talent="{$talent.spell}" data-tip-realm="{$realmId}" data-tip-rank="{$talent.points}" data-tip-max="{$talent.max_rank}"></a>
                                                        <div class="icon-border" id="{if $talent.points == 0}inactive{else}{if $talent.points == $talent.max_rank}maxed{else}active{/if}{/if}"></div>
                                                        <div class="icon-bubble" id="{if $talent.points == 0}inactive{else}{if $talent.points == $talent.max_rank}maxed{else}active{/if}{/if}">{$talent.points}</div>
                                                        {if $talent.points == 0}
                                                            <div class="overlay"></div>
                                                        {/if}
                                                        {if $talent.arrows|default:FALSE}
                                                            <!-- Dependency arrows -->
                                                            {foreach from=$talent.arrows key=arrow_key item=arrow}
                                                                <div {if $talent.points == $talent.max_rank}id="maxed"{/if} class="talent-dependency talent-dependency-{$arrow.pointing}" {if $arrow.pointing == "down"}style="height: {16 + $arrow.rows * 50}px;"{/if}>
                                                                    {if $arrow.pointing == "leftdown" || $arrow.pointing == "rightdown"}
                                                                        <div {if $talent.points == $talent.max_rank}id="maxed"{/if} class="talent-dependency-second" style="height: {27 + $arrow.rows * 50}px;"></div>
                                                                    {/if}
                                                                </div>
                                                            {/foreach}
                                                        {/if}
                                                    </div>
                                                {/if}
                                            </td>
                                        {/foreach}
                                    </tr>
                                {/foreach}
                            </table>
                            
                        </div>
                    {/foreach}
                        
                    <div class="clear"></div>
                </div>
                
                <h3>Glyphs</h3>
                <div class="talents-glyphs">
                	
                    {if $glyph_tables[$key]["hasPrime"]}
                        <!-- Prime Glyphs -->
                        <div class="talents-glyphs-column">
                            <div class="talents-glyphs-list">
                                
                                {for $glyph=0 to 2}
                                    <div class="talents-glyphs-glyph">
                                        <div class="iconsmall">
                                            <ins style="background-image: url(http://wow.zamimg.com/images/wow/icons/small/{$glyph_tables[$key].prime[$glyph].icon}.jpg);"></ins>
                                            <del></del>
                                           <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].prime[$glyph].id}" data-tip-type="Prime Glyph" data-tip-realm="{$realmId}"></a>
                                        </div>
                                        <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].prime[$glyph].id}" data-tip-type="Prime Glyph" data-tip-realm="{$realmId}" id="glyph-name">{$glyph_tables[$key].prime[$glyph].name}</a>
                                        <div class="clear"></div>
                                    </div>
                                {/for}
                                
                                <div class="clear"></div>
                            </div> 
                        </div>
                    {/if}
                    
                    <!-- Major Glyphs -->
                    <div class="talents-glyphs-column">
                        <div class="talents-glyphs-list">
                            
                            {for $glyph=0 to 2}
                                <div class="talents-glyphs-glyph">
                                    <div class="iconsmall">
                                        <ins style="background-image: url(http://wow.zamimg.com/images/wow/icons/small/{$glyph_tables[$key].major[$glyph].icon}.jpg);"></ins>
                                        <del></del>
                                       <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].major[$glyph].id}" data-tip-type="Major Glyph" data-tip-realm="{$realmId}"></a>
                                    </div>
                                    <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].major[$glyph].id}" data-tip-type="Major Glyph" data-tip-realm="{$realmId}" id="glyph-name">{$glyph_tables[$key].major[$glyph].name}</a>
                                    <div class="clear"></div>
                                </div>
                            {/for}
                            
                            <div class="clear"></div>
                        </div> 
                    </div>
                    
                    <!-- Minor Glyphs -->
                    <div class="talents-glyphs-column">
                        <div class="talents-glyphs-list">
                            
                            {for $glyph=0 to 2}
                                <div class="talents-glyphs-glyph">
                                    <div class="iconsmall">
                                        <ins style="background-image: url(http://wow.zamimg.com/images/wow/icons/small/{$glyph_tables[$key].minor[$glyph].icon}.jpg);"></ins>
                                        <del></del>
                                       <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].minor[$glyph].id}" data-tip-type="Minor Glyph" data-tip-realm="{$realmId}"></a>
                                    </div>
                                    <a href="javascript: void(0);" data-tip-glyph="{$glyph_tables[$key].minor[$glyph].id}" data-tip-type="Minor Glyph" data-tip-realm="{$realmId}" id="glyph-name">{$glyph_tables[$key].minor[$glyph].name}</a>
                                    <div class="clear"></div>
                                </div>
                            {/for}
                            
                            <div class="clear"></div>
                        </div> 
                    </div>
                    
                    <div class="clear"></div>
                </div>
            </div>
    	{/foreach}
    {/if}
    
</section>

<br />
<section id="armory_title" style="margin-top: 10px;">
    <div><h3>Arena Teams</h3></div>
</section>

<section id="armory_arena_teams">
	
    {foreach from=$arena_teams_table key=key item=team}
        <div class="armory_arena_box" {if not $team}id="inactive"{/if}>
            <div class="arena_box_head">
                <span id="team-type">{if $key == 0}2v2{/if}{if $key == 1}3v3{/if}{if $key == 2}5v5{/if}</span>
                {if $team}
                    <p id="player-rating"><a href="javascript: void(0);" data-tip="Personal Rating">{$team.player.rating}</a></p>
                    <p id="player-games">
                        <span id="player-wins">{$team.player.wins}</span> - <span id="player-loses">{$team.player.games - $team.player.wins}</span> 
                        {if $team.player.games > 0}
                            <span id="player-win-ratio">({round(($team.player.wins / $team.player.games) * 100)}%)</span>
                        {/if}
                    </p>
                {/if}
            </div>
            <div class="arena_box_body">
                {if $team}
                    <p id="team-name">{$team.teamName}</p>
                    <p id="team-stats">
                        <span id="team-rating"><a href="javascript: void(0);" data-tip="Team Rating">{$team.teamRating}</a></span> <span id="team-rank"><a href="javascript: void(0);" data-tip="Team Rank">#{$team.teamRank}</a></span>
                    </p>
                    <div id="team-members">
                        {if $team.members}
                            {foreach from=$team.members key=key item=member}
                                <a href="{$url}character/{$realmId}/{$member.guid}" data-tip="<font style='font-size: 16px; font-weight: bold;'>{$member.name}</font><br /><span class='color-c{$member.class}'>{$member.level} {$member.className} {$member.raceName}</span><br />{if $member.guildName}<span class='color-tooltip-{$member.faction}'>{$member.guildName}</span><br />{/if}Games won: {$member.wins}<br />Games lost: {$member.games - $member.wins}<br />Personal Rating: {$member.rating}" id="team-member">
                                    <img src='{$url}application/images/stats/{$member.class}.gif' align='absbottom'/>
                                </a>
                            {/foreach}
                        {/if}
                        <div class="clear"></div>
                    </div>
                {/if}
            </div>
        </div>
    {/foreach}
    
    <div class="clear"></div>
</section>

<!-- Load wowhead tooltip -->
{if !$fcms_tooltip}
<script type="text/javascript" src="https://static.wowhead.com/widgets/power.js"></script>
<script>var wowhead_tooltips = { "colorlinks": false, "iconizelinks": false, "renamelinks": false }</script>
{/if}

{if $TooltipPlayerData}
<script>
	var TooltipPlayerData = {json_encode($TooltipPlayerData)};
</script>
{/if}

{if $equippedItems}
<script>
	var TooltipEquippedItems = new Array();
	
	{foreach from=$equippedItems key=key item=entry}
	TooltipEquippedItems[{$key}] = {$entry};
	{/foreach}

</script>
{/if}