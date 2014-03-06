<div id="toppvp">
	
    {if $accounts}
        {foreach from=$accounts key=key item=account}
            <div class="toppvp_character">
                <div style="float:right">{$account.count} votes</div>
                <b>{$key + 1}</b>
                <a data-tip="View profile" href="{$url}profile/{$account.user_id}">{$account.nickname}</a> 
            </div>
        {/foreach}
    {else}
    <br />There are no voters to display<br /><br />
    {/if}
    
</div>