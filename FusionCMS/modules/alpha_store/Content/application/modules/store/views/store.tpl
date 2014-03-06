<section id="store_wrapper">
	<script type="text/javascript">
		$(document).ready(function()
		{
			function checkIfLoaded()
			{
				if(typeof Store != "undefined")
				{
					Store.Cart.initialize();
					Store.Customer.initialize({$vp}, {$dp});
					Store.Filter.initialize({$selected_realm}, {$filters.sort}, {$filters.quality}, {if $filters.search_encoded}'{$filters.search_encoded}'{else}'null'{/if}, {$filters.page});
					
					{if $cart}
						{foreach from=$cart item=cartitem}
							Store.Cart.add({$cartitem.id}, {$cartitem.info.itemid}, "{$cartitem.info.name}", {$cartitem.price}, "{$cartitem.priceType}", "{addslashes($cartitem.realm)}", {$selected_realm}, {$cartitem.info.quality}, {$cartitem.info.tooltip}, true, {$cartitem.count});
						{/foreach}
					{/if}
				}
				else
				{
					setTimeout(checkIfLoaded, 50);
				}
			}
			
			checkIfLoaded();
		});
	</script>

	<section id="checkout"></section>

	<section id="store">
    	<form onSubmit="Store.Filter.Search(); return false">
        	<section class="filter_row" id="first-row">
                <section class="filter_field">
                    <label for="realm">{$translation.realm}</label>
                    <select id="realm" name="realm" onChange="Store.Filter.setRealm(this.value)">
                    	{foreach from=$realms key=k item=realm}
                        	<option value="{$realm.id}" {if $selected_realm == $realm.id}selected="selected"{/if}>{$realm.name}</option>
                    	{/foreach}
                    </select>
                </section>
				<section class="filter_field">
                    <label for="sort_by">{lang("sort_by", "store")}</label>
                    <select id="sort_by" name="sort_by" onChange="Store.Filter.setSort(this.value)">
                        <option value="0" {if $filters.sort == 0}selected="selected"{/if}>{lang("default", "store")}</option>
                        <option value="1" {if $filters.sort == 1}selected="selected"{/if}>{lang("name", "store")}</option>
                        <option value="2" {if $filters.sort == 2}selected="selected"{/if}>{lang("price", "store")} ({lang("vp", "store")})</option>
                        <option value="3" {if $filters.sort == 3}selected="selected"{/if}>{lang("price", "store")} ({lang("dp", "store")})</option>
                        <option value="4" {if $filters.sort == 4}selected="selected"{/if}>{lang("item_quality", "store")}</option>
                    </select>
                </section>
                <section class="filter_field">
					<label for="item_quality">{$translation.quality}</label>
                    <select id="item_quality" name="item_quality" onChange="Store.Filter.setQuality(this.value)">
                        <option value="-1" {if $filters.quality == -1}selected="selected"{/if}>{lang("all_items", "store")}</option>
                        <option value="0" class="q0" {if $filters.quality == 0}selected="selected"{/if}>{lang("poor", "store")}</option>
                        <option value="1" class="q1" {if $filters.quality == 1}selected="selected"{/if}>{lang("common", "store")}</option>
                        <option value="2" class="q2" {if $filters.quality == 2}selected="selected"{/if}>{lang("uncommon", "store")}</option>
                        <option value="3" class="q3" {if $filters.quality == 3}selected="selected"{/if}>{lang("rare", "store")}</option>
                        <option value="4" class="q4" {if $filters.quality == 4}selected="selected"{/if}>{lang("epic", "store")}</option>
                        <option value="5" class="q5" {if $filters.quality == 5}selected="selected"{/if}>{lang("legendary", "store")}</option>
                        <option value="6" class="q6" {if $filters.quality == 6}selected="selected"{/if}>{lang("artifact", "store")}</option>
                        <option value="7" class="q7" {if $filters.quality == 7}selected="selected"{/if}>{lang("heirloom", "store")}</option>
                    </select>
                </section>
                <div class="clear"></div>
        	</section>
            
            <section class="filter_row" id="second-row">
                <section class="filter_field">
                    <input type="text" id="filter_name" placeholder="{lang("filter", "store")}" {if $filters.search}value="{$filters.search}"{/if} style="width: 100% !important;" />
                </section>
                <div class="clear"></div>
          	</section>
		</form>

		<div class="ucp_divider"></div>

		<section id="store_content">
			
			<section id="store_realms">
				<section class="realm_items" id="realm_items_{$selected_realm}">
				{if $data && $data.items}
					{foreach from=$data.items item=item key=itemId}
	
						<div class="store_item" id="item_{$item.id}">
							<section class="store_buttons">
								{if $item.vp_price}
									<a href="javascript:void(0)" onClick="Store.Cart.add({$item.id}, '{$item.itemid}', '{addslashes(preg_replace('/"/', "'", $item.name))}', {$item.vp_price}, 'vp', '{addslashes($item.realm_name)}', {$selected_realm}, {$item.quality}, {$item.tooltip})" class="nice_button vp_button">
										<img src="{$url}application/images/icons/lightning.png" align="absmiddle" /> <span class="vp_price_value">{$item.vp_price}</span> {lang("vp", "store")}
									</a>
									{/if}
	
									{if $item.dp_price}
									<a href="javascript:void(0)" onClick="Store.Cart.add({$item.id}, '{$item.itemid}', '{addslashes(preg_replace('/"/', "'", $item.name))}', {$item.dp_price}, 'dp', '{addslashes($item.realm_name)}', {$selected_realm}, {$item.quality}, {$item.tooltip})" class="nice_button dp_button">
										<img src="{$url}application/images/icons/coins.png" align="absmiddle" /> <span class="dp_price_value">{$item.dp_price}</span> {lang("dp", "store")}
									</a>
									{/if}
							</section>
	
							<img class="item_icon" src="https://wow.zamimg.com/images/wow/icons/medium/{$item.icon}.jpg" align="absmiddle" {if $item.tooltip}data-realm="{$selected_realm}" rel="item={$item.itemid}"{/if}>
							<a {if $item.tooltip}href="{$url}item/{$selected_realm}/{$item.itemid}" data-realm="{$selected_realm}" rel="item={$item.itemid}"{/if} class="item_name q{$item.quality}">
								{$item.name}
							</a>
							<br />{$item.description}
							<div class="clear"></div>
						</div>
	
					{/foreach}
				{else}
					<p id="empty_cart" class="alert-padding-fix">{$translation.no_items}</p>
				{/if}
				</section> <!-- realm end -->
			</section>
			
			{if $data.pagination}
			<section class="store-pagination store_buttons">
				{$data.pagination}
			</section>
			{/if}
			
			<div class="ucp_divider"></div>
			
			<section id="cart">
				<div class="online_realm_button">{lang("cart", "store")} (<span id="cart_item_count">0</span> {lang("items", "store")})</div>
				<div id="empty_cart">{lang("empty_cart", "store")}</div>
				<section id="cart_items"></section>

				<div id="cart_price">
					<div id="cart_price_divider"></div>

					<a href="javascript:void(0)" onClick="Store.Cart.checkout(this)" class="nice_button">{lang("checkout", "store")}</a>

					<div id="vp_price_full">
						<img src="{$url}application/images/icons/lightning.png" align="absmiddle" /> <span id="vp_price">0</span> {lang("vp", "store")}
					</div>

					<div id="dp_price_full">
						<img src="{$url}application/images/icons/coins.png" align="absmiddle" /> <span id="dp_price">0</span> {lang("dp", "store")}
					</div>

					<div class="clear"></div>
				</div>
			</section>
		</section>
		
	</section>
</section>