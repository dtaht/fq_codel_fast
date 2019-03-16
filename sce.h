static inline int INET_ECN_is_sce(__u8 dsfield)
{
	return (dsfield & INET_ECN_MASK) == INET_ECN_ECT_1;
}

static inline int IP_ECN_set_sce(struct iphdr *iph)
{
	u32 check = (__force u32)iph->check;
	u32 ecn = (iph->tos) & INET_ECN_MASK;

	if (ecn != INET_ECN_ECT_0)
		return !ecn;

	ipv4_change_dsfield(iph, INET_ECN_MASK, INET_ECN_ECT_1);
	return 1;
}

static inline int IP6_ECN_set_sce(struct sk_buff *skb, struct ipv6hdr *iph)
{
	__be32 from, to;
	u8 ecn = ipv6_get_dsfield(iph) & INET_ECN_MASK;

	if (ecn != INET_ECN_ECT_0)
		return !ecn;

	ipv6_change_dsfield(iph, INET_ECN_MASK, INET_ECN_ECT_1);
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
