/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:01:04 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 18:36:37 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_H
#define HISTORY_H

t_term	*copy_input_struct(t_term *current, int read);
t_history		*scroll_history_down(t_yank *buffer);
t_history		*scroll_history_up(t_yank *buffer);
void	cycling_hist(t_term *current, t_term *curs, t_actual *clone);

#endif