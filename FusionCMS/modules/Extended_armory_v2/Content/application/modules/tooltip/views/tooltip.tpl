<div style="max-width:350px;">
<span class='q{$item.quality}' style='font-size: 16px'>{$item.name}</span><br />

{if $item.isHeroic}<div class="q2">{$translation->item('heroic_flag')}</div>{/if}

{if $item.bind}{$item.bind}<br />{/if}
{if $item.unique}{$item.unique}<br />{/if}
{if $item.slot}<div style='float:left;'>{$item.slot}</div>{/if}
<div style='float:right;'>{$item.type}</div>
<div style="clear:both;"></div>
{if $item.armor}{$item.armor} {$translation->item('armor')}<br />{/if}

{if $item.damage_min}
	<div style='float:left;'>{$item.damage_min} - {$item.damage_max} {$item.damage_type} {$translation->item('damage')}</div>
	<div style='float:right;margin-left:15px;'>{$translation->item('speed')} {$item.speed}</div><br />
	({$item.dps} {$translation->item('dps')})<br />
{/if}

{if count($item.attributes.regular) > 0}
	{foreach from=$item.attributes.regular item=attribute}
		{$attribute.text}
	{/foreach}
{/if}

{if $item.holy_res}+ {$item.holy_res} {$translation->item('holy')}<br />{/if}
{if $item.nature_res}+ {$item.nature_res} {$translation->item('nature')}<br />{/if}
{if $item.fire_res}+ {$item.fire_res} {$translation->item('fire')}<br />{/if}
{if $item.frost_res}+ {$item.frost_res} {$translation->item('frost')}<br />{/if}
{if $item.shadow_res}+ {$item.shadow_res} {$translation->item('shadow')}<br />{/if}
{if $item.arcane_res}+ {$item.arcane_res} {$translation->item('arcane')}<br />{/if}

<div class="q2" id="tooltip-item-enchantments"></div>

<div id="tooltip-item-sockets">
{if $item.sockets}{$item.sockets}{/if}
</div>

{if $item.socketBonus}<div class="q0" id="tooltip-item-sock-bonus">{$translation->item('socket_bonus')}: {$item.socketBonus}</div>{/if}

{if $item.durability}{$translation->item('durability')} {$item.durability} / {$item.durability}<br />{/if}
{if $item.required}{$translation->item('requires_level')} {$item.required}<br />{/if}
{if $item.level}{$translation->item('item_level')} {$item.level}<br />{/if}

{if count($item.attributes.spells) > 0}
	{foreach from=$item.attributes.spells item=attribute}
		{$attribute.text}
	{/foreach}
{/if}

{if count($item.spells) > 0}
	{foreach from=$item.spells item=spell}
		<a class="q2" href="https://wowhead.com/?spell={$spell.id}" target="_blank">
			{$spell.trigger}
		
			{if !strlen($spell.text)}
				{$translation->item('unknown_effect')}
			{else}
				{$spell.text}
			{/if}
		</a>
		<br />
	{/foreach}
{/if}

{if $item.itemSet}
    <div id="tooltip-item-set" style="padding-top: 10px;">
        <div class="q" id="tooltip-item-set-name">{$item.itemSet.name} (<span id="tooltip-item-set-count">0</span>/{count($item.itemSet.items)})</div>
    	<div id="tooltip-item-set-pieces" style="padding-left: 5px;">
        	<ul style="list-style: none;" class="q0">
            	{foreach from=$item.itemSet.items item=setItem}
            	<li class="item-set-piece" data-itemset-item-entry="{$setItem.entry}" data-possible-entries="{$setItem.possibleItemEntries}">{$setItem.name}</li>
                {/foreach}
            </ul>
        </div>
        <div id="tooltip-item-set-bonuses" style="padding-top: 10px;">
        	<ul style="list-style: none;" class="q0">
            	{foreach from=$item.itemSet.setBonuses item=setBonus}
            	<li class="item-set-bonus" data-bonus-required-items="{$setBonus.requiredItems}">({$setBonus.requiredItems}) Set: <span id="set-bonus-text">{$setBonus.spell}</span></li>
                {/foreach}
            </ul>
        </div>
    </div>
{/if}
</div>