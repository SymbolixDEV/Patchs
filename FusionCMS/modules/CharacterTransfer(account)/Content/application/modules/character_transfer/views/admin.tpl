<section class="box big" id="donate_articles">
    <h2>
        <img src="{$url}application/themes/admin/images/icons/black16x16/ic_text_document.png"/>
        Latest transfer logs
    </h2>

    <form style="margin-top:0px;" onSubmit="CTA.search(); return false">
        <input type="text" name="string" id="search" placeholder="Search by character, source account or destination account" style="width:90%; margin-right:5px;"/>
        <input type="submit" value="Search" style="display:inline;padding:8px;" />
    </form>

    <ul id="logs_list">
        {if $logs}
            {foreach from=$logs item=log}
                <li>
                    <table width="100%" style="font-size:11px;">
                        <tr>
                            <td width="20%">{date("Y/m/d H:i:s", $log.time)}</td>
                            <td width="40%">
                                <a href="{$url}profile/{$log.account}" target="_blank">{$log.account_name}</a> transferred 
                                <a href="{$url}character/{$log.realm_id}/{$log.character}" target="_blank">{$log.character_name}</a><br />
                                from realm <strong>{$log.realmName}</strong><br /> 
                                to account <a href="{$url}profile/{$log.dest_account}" target="_blank">{$log.dest_account_name}</a>.
                            </td>
                            <td width="40%">
                            	The transfer costed <strong>{$log.price} {$log.currency}</strong>.<br />
                                Initial points <strong>{$log.start_currency}</strong>, after deduction <strong>{$log.end_currency}</strong>.
                            </td>
                        </tr>
                    </table>
                </li>
            {/foreach}
        {/if}
    </ul>
    
    {if $pagination}
    	<ul><li id="pagination"><center>{$pagination}</center></li></ul>
    {/if}
    
</section>