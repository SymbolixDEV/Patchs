{foreach from=$results item=log}
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