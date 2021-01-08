
#define SYS_NET_SUPP_INTF_${SYS_NET_SUPP_INTF}

#define SYS_NET_INDEX0_INTF       				SYS_NET_INTF_${SYS_NET_INTF}
#define SYS_NET_INDEX0_MODE       				SYS_NET_MODE_${SYS_NET_MODE}
#define SYS_NET_INDEX0_PORT        				${SYS_NET_PORT}
#define SYS_NET_INDEX0_RECONNECT       			${SYS_NET_RECONNECT?string(1, 0)}
#define SYS_NET_INDEX0_ENABLE_TLS        		${SYS_NET_ENABLE_TLS?string(1, 0)}
#define SYS_NET_INDEX0_IPPROT       			SYS_NET_IP_PROT_${SYS_NET_IPPROT}
#define SYS_NET_INDEX0_HOST_NAME        		"${SYS_NET_HOST_NAME}"


<#if SYS_NET_IDX1 == true>
#define SYS_NET_VALID_INST1
#define SYS_NET_INDEX1_INTF       				SYS_NET_INTF_${SYS_NET_IDX1_INTF}
#define SYS_NET_INDEX1_MODE       				SYS_NET_MODE_${SYS_NET_IDX1_MODE}
#define SYS_NET_INDEX1_PORT        				${SYS_NET_IDX1_PORT}
#define SYS_NET_INDEX1_RECONNECT       			${SYS_NET_IDX1_RECONNECT?string(1, 0)}
#define SYS_NET_INDEX1_ENABLE_TLS        		${SYS_NET_IDX1_ENABLE_TLS?string(1, 0)}
#define SYS_NET_INDEX1_IPPROT       			SYS_NET_IP_PROT_${SYS_NET_IDX1_IPPROT}
#define SYS_NET_INDEX1_HOST_NAME        		"${SYS_NET_IDX1_HOST_NAME}"
</#if>

<#if SYS_NET_ENABLE_TLS == true>
#define SYS_NET_TLS_ENABLED
</#if>

<#if SYS_NET_IDX1_ENABLE_TLS == true>
#define SYS_NET_TLS_ENABLED
</#if>

<#if SYS_NET_ENABLE_DEBUG == true>
<#if SYS_NET_ENABLE_CLICMD == true>
#define SYS_NET_CLICMD_ENABLED
</#if>

<#if SYS_NET_APPDEBUG_ENABLE == true>
#define SYS_NET_ENABLE_DEBUG_PRINT						
#define SYS_NET_DEBUG_PRESTR						"${SYS_NET_APPDEBUG_PRESTR}"

<#if SYS_NET_APPDEBUG_ERR_LEVEL == true>
#define SYS_NET_APPDEBUG_ERR_LEVEL_ENABLE						
</#if>
<#if SYS_NET_APPDEBUG_DBG_LEVEL == true>
#define SYS_NET_APPDEBUG_DBG_LEVEL_ENABLE						
</#if>
<#if SYS_NET_APPDEBUG_INFO_LEVEL == true>
#define SYS_NET_APPDEBUG_INFO_LEVEL_ENABLE						
</#if>
<#if SYS_NET_APPDEBUG_FUNC_LEVEL == true>
#define SYS_NET_APPDEBUG_FUNC_LEVEL_ENABLE						
</#if>

<#if SYS_NET_APPDEBUG_CFG_FLOW == true>
#define SYS_NET_APPDEBUG_CFG_FLOW_ENABLE						
</#if>
<#if SYS_NET_APPDEBUG_DATA_FLOW == true>
#define SYS_NET_APPDEBUG_DATA_FLOW_ENABLE						
</#if>
</#if>
</#if>

