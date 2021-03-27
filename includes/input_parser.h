/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aophion <aophion@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:01:21 by aophion           #+#    #+#             */
/*   Updated: 2021/03/27 15:28:23 by aophion          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_PARSER_H
# define INPUT_PARSER_H



int		parse_incoming_subline(t_norm *norm, int size);
int		verify_char_heredoc(char c);
char	find_next_char(char *str, int i);
int		check_for_zero(char *str, int i);
int		verify_end_arg(char c, int n, int state);
int		modify_ignored(t_norm *norm, int state, int *flag);
int		state_save_zero_hdoc(t_norm *norm, int state);
int		handle_heredoc(t_norm *norm, int state);
extern void			save_coord_hdoc(t_norm *norm);
#endif
