<script type="text/javascript">
	$(document).ready(function()
	{
		function initializeCharacterTools()
		{
			if(typeof CharacterTools != "undefined")
			{
				CharacterTools.User.initialize({$dp});
			}
			else
			{
				setTimeout(initializeCharacterTools, 50);
			}
		}

		initializeCharacterTools();
	});
</script>
<section id="character_tools">
	<section id="select_character">
		<div class="online_realm_button">{lang("select_char", "teleport")}</div>
		
		{if $total}
			{foreach from=$characters item=realm}
				<div class="teleport_realm_divider">{$realm.realmName}</div>
				{foreach from=$realm.characters item=character}
					<div class="select_character">
						<div class="character store_item">
							<section class="character_buttons">
								<a href="javascript:void(0)" class="nice_button" onClick="CharacterTools.selectCharacter(this, {$realm.realmId}, {$character.guid}, '{$character.name}')">
									{lang("select", "teleport")}
								</a>
							</section>
			
							<img class="item_icon" width="36" height="36" src="{$url}application/images/avatars/{$character.avatar}.gif" align="absmiddle" data-tip="<img src='{$url}application/images/stats/{$character.class}.gif' align='absbottom'/> {$character.name} (Lv{$character.level})">
			
							<a class="character_name" data-tip="<img src='{$url}application/images/stats/{$character.class}.gif' align='absbottom'/> {$character.name} (Lv{$character.level})">{$character.name}</a>
							<br />Level {$character.level}
							<div class="clear"></div>
						</div>
					</div>
				{/foreach}
			{/foreach}
		{else}
			<center style="padding-top:10px;"><b>{lang("no_chars", "teleport")}</b></center>
		{/if}
	</section>
    
	<section id="select_tool">
    	<div class="online_realm_button">Select service</div>
        
        <div class="select_tools">
        
            <!-- Character Rename -->
            <div class="select_tool">
                <div class="tool store_item">
                    <section class="tool_buttons">
                        <a href="javascript:void(0)" class="nice_button" onClick="CharacterTools.Purchase(this, 1, {$config->item('name_change_price')})">
                            Purchase
                        </a>
                    </section>
    
                    <img class="item_icon" data-tip="Change your characters’ names" src="{$url}application/modules/character_tools/css/images/inv_misc_note_06.jpg" width="36" height="36" src="" align="absmiddle">
    
                    <a class="tool_name" data-tip="Change your characters’ names">Name Change</a>
                    <br />
                    {if $config->item('name_change_price') > 0}Cost: {$config->item('name_change_price')} Donation Points{else}Free of charge{/if}
                    <div class="clear"></div>
                </div>
            </div>
            
            <!-- Race Change -->
            <div class="select_tool">
                <div class="tool store_item">
                    <section class="tool_buttons">
                        <a href="javascript:void(0)" class="nice_button" onClick="CharacterTools.Purchase(this, 2, {$config->item('race_change_price')})">
                            Purchase
                        </a>
                    </section>
    
                    <img class="item_icon" data-tip="Change a character’s race (within your current faction)" src="{$url}application/modules/character_tools/css/images/race_change.jpg" width="36" height="36" src="" align="absmiddle">
    
                    <a class="tool_name" data-tip="Change a character’s race (within your current faction)">Race Change</a>
                    <br />
                    {if $config->item('race_change_price') > 0}Cost: {$config->item('race_change_price')} Donation Points{else}Free of charge{/if}
                    <div class="clear"></div>
                </div>
            </div>
            
            <!-- Faction Change -->
            <div class="select_tool">
                <div class="tool store_item">
                    <section class="tool_buttons">
                        <a href="javascript:void(0)" class="nice_button" onClick="CharacterTools.Purchase(this, 3, {$config->item('faction_change_price')})">
                            Purchase
                        </a>
                    </section>
    
                    <img class="item_icon" data-tip="Change a character’s faction (Horde to Alliance or Alliance to Horde)" src="{$url}application/modules/character_tools/css/images/faction_change.jpg" width="36" height="36" src="" align="absmiddle">
    
                    <a class="tool_name" data-tip="Change a character’s faction (Horde to Alliance or Alliance to Horde)">Faction Change</a>
                    <br />
                    {if $config->item('faction_change_price') > 0}Cost: {$config->item('faction_change_price')} Donation Points{else}Free of charge{/if}
                    <div class="clear"></div>
                </div>
            </div>
            
            <!-- Appearance Change -->
            <div class="select_tool">
                <div class="tool store_item">
                    <section class="tool_buttons">
                        <a href="javascript:void(0)" class="nice_button" onClick="CharacterTools.Purchase(this, 4, {$config->item('appearance_change_price')})">
                            Purchase
                        </a>
                    </section>
    
                    <img class="item_icon" data-tip="Change your characters’ appearance (optional name change included)" src="{$url}application/modules/character_tools/css/images/achievement_character_human_female.jpg" width="36" height="36" src="" align="absmiddle">
    
                    <a class="tool_name" data-tip="Change your characters’ appearance (optional name change included)">Appearance Change</a>
                    <br />
                    {if $config->item('appearance_change_price') > 0}Cost: {$config->item('appearance_change_price')} Donation Points{else}Free of charge{/if}
                    <div class="clear"></div>
                </div>
            </div>
            
            <div class="clear"></div>
        </div>
		<div class="clear"></div>
        
		<div class="ucp_divider"></div>
        
	</section>

	<div class="clear"></div>
</section>