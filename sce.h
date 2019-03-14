static inline int INET_ECN_is_sce(__u8 dsfield)
{
	return (dsfield & INET_ECN_MASK) == INET_ECN_ECT_0;
}

static inline int IP_ECN_set_sce(struct iphdr *iph)
{
	u32 check = (__force u32)iph->check;
	u32 ecn = (iph->tos) & INET_ECN_MASK;

	if (!(ecn == INET_ECN_ECT_0))
		return !ecn;

	ecn = INET_ECN_ECT_1;

	/*
	 * The following gives us:
	 * INET_ECN_ECT_1 => check += htons(0xFFFD)
	 * INET_ECN_ECT_0 => check += htons(0xFFFE)
	 */
	// FIXME: I do not understand this line
	
	check += (__force u16)htons(0xFFFB) + (__force u16)htons(ecn);

	iph->check = (__force __sum16)(check + (check>=0xFFFF));
	iph->tos |= INET_ECN_ECT_1;
	return 1;
}

static inline int IP6_ECN_set_sce(struct sk_buff *skb, struct ipv6hdr *iph)
{
	__be32 from, to;

	if (INET_ECN_is_not_ect(ipv6_get_dsfield(iph)))
		return 0;

	from = *(__be32 *)iph;
	to = from | htonl(INET_ECN_CE << 20);
	*(__be32 *)iph = to;
	if (skb->ip_summed == CHECKSUM_COMPLETE)
		skb->csum = csum_add(csum_sub(skb->csum, (__force __wsum)from),
				     (__force __wsum)to);
	return 1;
}


static inline int INET_ECN_set_sce(struct sk_buff *skb)
{
	switch (skb->protocol) {
	case cpu_to_be16(ETH_P_IP):
		if (skb_network_header(skb) + sizeof(struct iphdr) <=
		    skb_tail_pointer(skb))
			return IP_ECN_set_sce(ip_hdr(skb));
		break;

	case cpu_to_be16(ETH_P_IPV6):
		if (skb_network_header(skb) + sizeof(struct ipv6hdr) <=
		    skb_tail_pointer(skb))
			return IP6_ECN_set_sce(skb, ipv6_hdr(skb));
		break;
	}

	return 0;
}
