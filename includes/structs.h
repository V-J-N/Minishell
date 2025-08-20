/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: serjimen <serjimen@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:30:20 by serjimen          #+#    #+#             */
/*   Updated: 2025/08/19 23:43:32 by serjimen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

/**
 * @struct t_env
 * @brief Nodo de la lista enlazada que representa un variablle de entorno.
 * 
 */
typedef struct s_env
{
	char			*key;
	char			*value;
	char			*full_env;
	struct s_env	*next;
}					t_env;

#endif