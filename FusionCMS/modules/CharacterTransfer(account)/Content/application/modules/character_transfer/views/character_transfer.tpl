<script type="text/javascript">
	function Initialize()
	{
		if (typeof CharacterTransfer != "undefined")
		{
			CharacterTransfer.Initialize({$first_realm.id});
			CharacterTransfer.LANG_SELECT_CHAR = '{$cta_lang.SELECT_CHAR}';
			CharacterTransfer.LANG_SELECT_DEST = '{$cta_lang.ENTER_DEST_ACC}';
		}
		else
		{
			setTimeout(Initialize, 100);
		}
	}

	$(document).ready(function()
	{
		Initialize();
	});
</script>
<section id="character_transfer">
	<div id="cta_description">{$cta_lang.DESCRIPTION}</div>
    
    <div id="cta_body">
    	
        <div class="cta_body_left">
        
        	<div class="cta_row">
                <span>{$cta_lang.REALM}:</span>
                <select name="realm" onchange="return CharacterTransfer.RealmChange(this);">
                    {foreach from=$realms key=k item=realm}
                        <option value="{$realm.id}">{$realm.name}</option>
                    {/foreach}
                </select>
          	</div>
            
            {foreach from=$realms key=k item=realm}
            <div class="cta_row" {if $realm.id != $first_realm.id}style="display: none;"{else}id="cta_visible_char_select"{/if}>
                <span>{$cta_lang.CHARACTER}:</span>
                <select name="characters_{$realm.id}" id="cta_char_select_{$realm.id}">
                    <option disabled="disabled" selected="selected">{$cta_lang.PLS_SELECT}</option>
                    {if $realm.characters}
                        {foreach from=$realm.characters key=k2 item=char}
                            <option value="{$char.name}">{$char.name} - {$char.level} level</option>
                        {/foreach}
                  	{/if}
                </select>
          	</div>
            {/foreach}
            
        </div>
        <div class="cta_body_middle"></div>
        <div class="cta_body_right">
        	<form onsubmit="CharacterTransfer.Submit(); return false;">
                <div class="cta_row">
                    <span>{$cta_lang.DEST_ACC}:</span>
                    <input type="text" name="account" id="cta_dest_account" onkeyup="return CharacterTransfer.DestAccountUpdate(this);" />
                </div>
                
                <div class="cta_row">
                    <p>{$cta_lang.COST_EXPL}</p>
                </div>
                
                <div class="cta_row" align="center">
                    <input type="submit" value="{$cta_lang.TRANSFER}" />
                </div>
            </form>
        </div>
        
        <div class="clear"></div>
    </div>
</section>
