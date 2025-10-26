/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vjan-nie <vjan-nie@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 05:31:01 by vjan-nie          #+#    #+#             */
/*   Updated: 2025/10/26 07:58:03 by vjan-nie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file global.c
 * @brief Definition of the global signal status variable.
 */
#include "minishell.h"

/**
 * @brief Global variable to track the status of received signals.
 * This variable is set by the signal handler and read by the main
 * REPL loop to determine if the last command was terminated by an interrupt.
 */
volatile sig_atomic_t	g_exit_code = 0;